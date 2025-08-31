#include "managers/AssetManager.h"

#include <spdlog/spdlog.h>

#include <memory>

#include "utils/PMDUtils.h"

AssetManager::AssetManager(const std::filesystem::path& assetRoot)
    : m_assetRoot(assetRoot),
      m_pmdCollabPath(assetRoot / "pmdcollab"),
      m_portraitPath(assetRoot / "pmdcollab" / "portrait") {}

AssetManager::~AssetManager() {
  for (auto const& [path, texture] : m_textureCache) {
    UnloadTexture(texture);
  }
  m_textureCache.clear();
}

bool AssetManager::loadPokemonSpriteData(const std::string& dexNumber) {
  if (m_loadedForms.count(dexNumber)) return true;

  if (!m_trackerLoaded) {
    const auto trackerPath = m_pmdCollabPath / "tracker.json";
    spdlog::debug("Loading tracker file: {}", trackerPath.string());
    std::string buffer;
    auto error = glz::read_file_json<glz::opts{.error_on_unknown_keys = false}>(
        m_trackerData, trackerPath.string(), buffer);
    if (error) {
      spdlog::error("Failed to parse tracker.json: {}", glz::format_error(error, buffer));
      return false;
    }
    m_trackerLoaded = true;
    spdlog::info("Tracker.json loaded successfully");
  }

  auto it = m_trackerData.find(dexNumber);
  if (it == m_trackerData.end()) {
    spdlog::error("Pokémon #{} not found in tracker.json", dexNumber);
    return false;
  }

  const TrackerEntry& baseEntry = it->second;
  processTrackerEntry(dexNumber, "", baseEntry, "", std::filesystem::path());
  return true;
}

void AssetManager::processTrackerEntry(const std::string& dex, const std::string& subgroupId,
                                       const TrackerEntry& entry, const std::string& parentName,
                                       const std::filesystem::path& parentPath) {
  std::string currentFullName = PMDUtils::generateFullName(parentName, entry.name);
  std::filesystem::path currentRelativePath = parentPath / subgroupId;
  std::string currentFullId = PMDUtils::generateFullId(dex, currentRelativePath);

  auto newForm = std::make_shared<PMDData>();
  newForm->dex = dex;
  newForm->fullId = currentFullId;
  newForm->fullName = currentFullName;
  newForm->formPath = currentRelativePath.string();
  newForm->spriteCredit = entry.sprite_credit;
  newForm->portraitCredit = entry.portrait_credit;

  std::filesystem::path animDataPath =
      m_pmdCollabPath / "sprite" / dex / currentRelativePath / "AnimData.xml";
  newForm->animData = PMDUtils::parseAnimationData(animDataPath);

  std::filesystem::path spritePath = m_pmdCollabPath / "sprite" / dex / currentRelativePath;
  if (std::filesystem::exists(spritePath) && std::filesystem::is_directory(spritePath)) {
    const std::string animSuffix = "-Anim.png";
    for (const auto& dir_entry : std::filesystem::directory_iterator{spritePath}) {
      if (dir_entry.is_regular_file()) {
        std::string filename = dir_entry.path().filename().string();
        if (filename.ends_with(animSuffix)) {
          std::string baseName = filename.substr(0, filename.length() - animSuffix.length());
          newForm->animFileBases.push_back(baseName);
        }
      }
    }
  }

  std::filesystem::path portraitFormPath = m_portraitPath / dex / currentRelativePath;

  if (std::filesystem::exists(portraitFormPath) &&
      std::filesystem::is_directory(portraitFormPath)) {
    for (const auto& dir_entry : std::filesystem::directory_iterator{portraitFormPath}) {
      if (dir_entry.is_regular_file() && dir_entry.path().extension() == ".png") {
        newForm->availablePortraits.insert(dir_entry.path().stem().string());
      }
    }
  }

  if (newForm->animData || !newForm->availablePortraits.empty()) {
    m_loadedForms[currentFullId] = newForm;
    spdlog::info("Loaded form: {} ({})", newForm->fullName, newForm->fullId);
  }

  for (const auto& [id, subEntry] : entry.subgroups) {
    processTrackerEntry(dex, id, subEntry, currentFullName, currentRelativePath);
  }
}

std::shared_ptr<const PMDData> AssetManager::getForm(const std::string& fullId) const {
  auto it = m_loadedForms.find(fullId);
  return (it != m_loadedForms.end()) ? it->second : nullptr;
}

Texture2D AssetManager::getAnimationTexture(const std::string& formId,
                                            const std::string& animationName) {
  auto form = getForm(formId);
  if (!form) {
    spdlog::error("Form not found: {}", formId);
    return Texture2D{0};
  }

  std::string baseName = PMDUtils::findAnimationBaseName(*form, animationName);

  if (baseName.empty()) {
    spdlog::error("No texture file for animation '{}' in form '{}'", animationName, form->fullName);
    return Texture2D{0};
  }

  std::filesystem::path texturePath =
      m_pmdCollabPath / "sprite" / form->dex / form->formPath / (baseName + "-Anim.png");

  std::string pathString = texturePath.string();

  if (m_textureCache.count(pathString)) {
    return m_textureCache.at(pathString);
  }

  if (!std::filesystem::exists(texturePath)) {
    spdlog::error("Texture file not found: {}", pathString);
    return Texture2D{0};
  }

  Texture2D texture = LoadTexture(pathString.c_str());
  if (texture.id > 0) {
    spdlog::debug("Loaded texture: {}", pathString);
    m_textureCache[pathString] = texture;
  } else {
    spdlog::error("Failed to load texture: {}", pathString);
  }
  return texture;
}
