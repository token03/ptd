#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "data/PMDData.h"
#include "managers/DataManager.h"
#include "raylib.h"

class TextureManager {
 public:
  TextureManager(std::shared_ptr<DataManager> dataManager);
  ~TextureManager();

  std::shared_ptr<const PMDData> getForm(const std::string &name) const;
  const std::map<std::string, std::shared_ptr<PMDData>> &getAllForms() const {
    return m_dataManager->getAllForms();
  }

  Texture2D getAnimationTexture(const std::string &name,
                                const std::string &animationName);
  Texture2D getBackgroundTexture(const std::string &bgName);
  Texture2D getPortraitTexture(const std::string &name, const std::string &portraitName);
  Texture2D getIconSheetTexture();
  Rectangle getIconSourceRect(int iconIndex) const;

 private:
  Texture2D getOrLoadTexture(const std::filesystem::path &texturePath);

 private:
  std::shared_ptr<DataManager> m_dataManager;
  std::filesystem::path m_assetRoot;
  std::filesystem::path m_pmdCollabPath;
  std::filesystem::path m_portraitPath;
  std::filesystem::path m_backgroudPath;
  std::filesystem::path m_smogonPath;

  std::map<std::string, Texture2D> m_textureCache;

  Texture2D m_iconSheetTexture;
  bool m_iconSheetLoaded = false;

  static constexpr int ICON_WIDTH = 40;
  static constexpr int ICON_HEIGHT = 30;
  static constexpr int ICONS_PER_ROW = 12;
};
