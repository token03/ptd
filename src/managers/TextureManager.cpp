#include "managers/TextureManager.h"

#include <spdlog/spdlog.h>

#include <memory>

#include "utils/PMDUtils.h"

TextureManager::TextureManager(std::shared_ptr<DataManager> dataManager)
    : m_dataManager(std::move(dataManager)),
      m_assetRoot("assets"),
      m_pmdCollabPath(m_assetRoot / "pmdcollab"),
      m_portraitPath(m_pmdCollabPath / "portrait"),
      m_backgroudPath(m_assetRoot / "backgrounds"),
      m_smogonPath(m_assetRoot / "smogon") {
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

TextureManager::~TextureManager() {
  for (auto const &[path, texture] : m_textureCache) {
    UnloadTexture(texture);
  }
  m_textureCache.clear();
  if (m_iconSheetLoaded) {
    UnloadTexture(m_iconSheetTexture);
  }
}

std::shared_ptr<const PMDData> TextureManager::getForm(const std::string &name) const {
  const auto &allForms = m_dataManager->getAllForms();
  auto it = allForms.find(name);
  return (it != allForms.end()) ? it->second : nullptr;
}

Texture2D TextureManager::getAnimationTexture(const std::string &name,
                                              const std::string &animationName) {
  auto form = getForm(name);
  if (!form) {
    spdlog::error("Form not found for name: {}", name);
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

Texture2D TextureManager::getPortraitTexture(const std::string &name,
                                             const std::string &portraitName) {
  auto form = getForm(name);
  if (!form) {
    spdlog::error("Form not found for portrait: {}", name);
    return Texture2D{0};
  }

  std::filesystem::path texturePath =
      m_portraitPath / form->dex / form->formPath / (portraitName + ".png");

  return getOrLoadTexture(texturePath);
}

Texture2D TextureManager::getIconSheetTexture() { return m_iconSheetTexture; }

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
