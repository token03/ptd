#pragma once

#include "data/PMDData.h"
#include "pugixml.hpp"
#include "raylib.h"
#include <filesystem>
#include <map>
#include <memory>
#include <string>

class AssetManager {
public:
  AssetManager(const std::filesystem::path &assetRoot);
  ~AssetManager();

  bool loadPokemonSpriteData(const std::string &dexNumber);
  std::shared_ptr<const PMDData> getForm(const std::string &fullId) const;
  const std::map<std::string, std::shared_ptr<PMDData>> &getAllForms() const {
    return m_loadedForms;
  }

  Texture2D getAnimationTexture(const std::string &formId,
                                const std::string &animationName);
  std::string findAnimationBaseName(const PMDData &form,
                                    const std::string &animationName);

private:
  void processTrackerEntry(const std::string &dex,
                           const std::string &subgroupId,
                           const TrackerEntry &entry,
                           const std::string &parentName,
                           const std::filesystem::path &parentPath);

  std::optional<AnimationData>
  parseAnimationData(const std::filesystem::path &xmlPath);

private:
  std::filesystem::path m_assetRoot;
  std::filesystem::path m_pmdCollabPath;
  std::filesystem::path m_portraitPath;
  std::map<std::string, std::shared_ptr<PMDData>> m_loadedForms;

  std::map<std::string, TrackerEntry> m_trackerData;
  bool m_trackerLoaded = false;

  std::map<std::string, Texture2D> m_textureCache;
};
