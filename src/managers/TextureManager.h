#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "data/PMDData.h"
#include "raylib.h"

class TextureManager {
 public:
  TextureManager(const std::filesystem::path& assetRoot);
  ~TextureManager();

  bool loadPokemonSpriteData(const std::string& dexNumber);
  std::shared_ptr<const PMDData> getForm(const std::string& fullId) const;
  const std::map<std::string, std::shared_ptr<PMDData>>& getAllForms() const {
    return m_loadedForms;
  }

  Texture2D getAnimationTexture(const std::string& formId,
                                const std::string& animationName);
  Texture2D getBackgroundTexture(const std::string& bgName);
  Texture2D getPortraitTexture(const std::string& formId,
                               const std::string& portraitName);
  Texture2D getIconTexture(const std::string& speciesName);

 private:
  void processTrackerEntry(const std::string& dex, const std::string& subgroupId,
                           const TrackerEntry& entry, const std::string& parentName,
                           const std::filesystem::path& parentPath);
  bool ensureTrackerLoaded();
  Texture2D getOrLoadTexture(const std::filesystem::path& texturePath);

 private:
  std::filesystem::path m_assetRoot;
  std::filesystem::path m_pmdCollabPath;
  std::filesystem::path m_portraitPath;
  std::filesystem::path m_backgroudPath;
  std::map<std::string, std::shared_ptr<PMDData>> m_loadedForms;

  std::map<std::string, TrackerEntry> m_trackerData;
  bool m_trackerLoaded = false;

  std::map<std::string, Texture2D> m_textureCache;
};
