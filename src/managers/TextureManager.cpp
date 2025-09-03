#include "managers/TextureManager.h"

#include <spdlog/spdlog.h>

#include <memory>
#include <set>
#include <vector>

#include "utils/PMDUtils.h"

namespace {
std::vector<std::string> findAnimationBases(const std::filesystem::path &spriteDir) {
  std::vector<std::string> bases;
  if (!std::filesystem::exists(spriteDir) || !std::filesystem::is_directory(spriteDir)) {
    return bases;
  }
  const std::string animSuffix = "-Anim.png";
  for (const auto &dir_entry : std::filesystem::directory_iterator{spriteDir}) {
    if (dir_entry.is_regular_file()) {
      std::string filename = dir_entry.path().filename().string();
      if (filename.ends_with(animSuffix)) {
        bases.push_back(filename.substr(0, filename.length() - animSuffix.length()));
      }
    }
  }
  return bases;
}

std::set<std::string> findAvailablePortraits(const std::filesystem::path &portraitDir) {
  std::set<std::string> portraits;
  if (!std::filesystem::exists(portraitDir) ||
      !std::filesystem::is_directory(portraitDir)) {
    return portraits;
  }
  for (const auto &dir_entry : std::filesystem::directory_iterator{portraitDir}) {
    if (dir_entry.is_regular_file() && dir_entry.path().extension() == ".png") {
      portraits.insert(dir_entry.path().stem().string());
    }
  }
  return portraits;
}
}  // namespace

TextureManager::TextureManager()
    : m_assetRoot("assets"),
      m_pmdCollabPath(m_assetRoot / "pmdcollab"),
      m_portraitPath(m_assetRoot / "pmdcollab" / "portrait"),
      m_backgroudPath(m_assetRoot / "backgrounds"),
      m_smogonPath(m_assetRoot / "smogon") {}

TextureManager::~TextureManager() {
  for (auto const &[path, texture] : m_textureCache) {
    UnloadTexture(texture);
  }
  m_textureCache.clear();
  if (m_iconSheetLoaded) {
    UnloadTexture(m_iconSheetTexture);
  }
}

bool TextureManager::loadPokemonSpriteData(const std::string &dexNumber) {
  if (m_loadedForms.count(dexNumber)) {
    return true;
  }

  if (!ensureTrackerLoaded()) {
    return false;
  }

  auto it = m_trackerData.find(dexNumber);
  if (it == m_trackerData.end()) {
    spdlog::error("Pokémon #{} not found in tracker.json", dexNumber);
    return false;
  }

  const TrackerEntry &baseEntry = it->second;
  processTrackerEntry(dexNumber, "", baseEntry, "", std::filesystem::path());
  return true;
}

void TextureManager::processTrackerEntry(const std::string &dex,
                                         const std::string &subgroupId,
                                         const TrackerEntry &entry,
                                         const std::string &parentName,
                                         const std::filesystem::path &parentPath) {
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

  std::filesystem::path baseSpritePath =
      m_pmdCollabPath / "sprite" / dex / currentRelativePath;
  newForm->animData = PMDUtils::parseAnimationData(baseSpritePath / "AnimData.xml");
  newForm->animFileBases = findAnimationBases(baseSpritePath);

  std::filesystem::path portraitFormPath = m_portraitPath / dex / currentRelativePath;
  newForm->availablePortraits = findAvailablePortraits(portraitFormPath);

  if (newForm->animData || !newForm->availablePortraits.empty()) {
    m_loadedForms[currentFullId] = newForm;
    spdlog::info("Loaded form: {} ({})", newForm->fullName, newForm->fullId);
  }

  for (const auto &[id, subEntry] : entry.subgroups) {
    processTrackerEntry(dex, id, subEntry, currentFullName, currentRelativePath);
  }
}

std::shared_ptr<const PMDData> TextureManager::getForm(const std::string &fullId) const {
  auto it = m_loadedForms.find(fullId);
  return (it != m_loadedForms.end()) ? it->second : nullptr;
}

Texture2D TextureManager::getAnimationTexture(const std::string &formId,
                                              const std::string &animationName) {
  auto form = getForm(formId);
  if (!form) {
    spdlog::error("Form not found: {}", formId);
    return Texture2D{0};
  }

  std::string baseName = PMDUtils::findAnimationBaseName(*form, animationName);

  if (baseName.empty()) {
    spdlog::error("No texture file for animation '{}' in form '{}'", animationName,
                  form->fullName);
    return Texture2D{0};
  }

  std::filesystem::path texturePath =
      m_pmdCollabPath / "sprite" / form->dex / form->formPath / (baseName + "-Anim.png");

  return getOrLoadTexture(texturePath);
}

Texture2D TextureManager::getBackgroundTexture(const std::string &bgName) {
  std::filesystem::path texturePath = m_backgroudPath / (bgName + ".png");
  return getOrLoadTexture(texturePath);
}

Texture2D TextureManager::getPortraitTexture(const std::string &formId,
                                             const std::string &portraitName) {
  auto form = getForm(formId);
  if (!form) {
    spdlog::error("Form not found for portrait: {}", formId);
    return Texture2D{0};
  }

  std::filesystem::path texturePath =
      m_portraitPath / form->dex / form->formPath / (portraitName + ".png");

  return getOrLoadTexture(texturePath);
}

void TextureManager::ensureIconSheetLoaded() {
  if (m_iconSheetLoaded) {
    return;
  }

  const auto iconSheetPath = m_smogonPath / "pokemon-icons.png";
  if (!std::filesystem::exists(iconSheetPath)) {
    spdlog::error("Icon sheet texture not found: {}", iconSheetPath.string());
    m_iconSheetTexture = Texture2D{0};
    return;
  }

  m_iconSheetTexture = LoadTexture(iconSheetPath.string().c_str());
  if (m_iconSheetTexture.id > 0) {
    spdlog::debug("Loaded icon sheet texture: {}", iconSheetPath.string());
    m_iconSheetLoaded = true;
  } else {
    spdlog::error("Failed to load icon sheet texture: {}", iconSheetPath.string());
  }
}

Texture2D TextureManager::getIconSheetTexture() {
  ensureIconSheetLoaded();
  return m_iconSheetTexture;
}

Rectangle TextureManager::getIconSourceRect(int iconIndex) const {
  if (iconIndex < 0) {
    spdlog::warn("Requested negative icon index: {}", iconIndex);
    return {0, 0, (float)ICON_WIDTH, (float)ICON_HEIGHT};
  }
  const int col = iconIndex % ICONS_PER_ROW;
  const int row = iconIndex / ICONS_PER_ROW;

  return {(float)(col * ICON_WIDTH), (float)(row * ICON_HEIGHT), (float)ICON_WIDTH,
          (float)ICON_HEIGHT};
}

bool TextureManager::ensureTrackerLoaded() {
  if (m_trackerLoaded) {
    return true;
  }

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
  return true;
}

Texture2D TextureManager::getOrLoadTexture(const std::filesystem::path &texturePath) {
  std::string pathString = texturePath.string();

  auto it = m_textureCache.find(pathString);
  if (it != m_textureCache.end()) {
    return it->second;
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
