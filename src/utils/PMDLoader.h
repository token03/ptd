#pragma once

#include "pugixml.hpp"
#include "raylib.h"
#include "utils/PMDData.h"
#include <filesystem>
#include <map>
#include <string>

class PMDLoader {
public:
  PMDLoader(const std::filesystem::path &assetRoot);
  ~PMDLoader();

  bool loadPokemon(const std::string &dexNumber);
  const PokemonForm *getForm(const std::string &fullId) const;
  const std::map<std::string, PokemonForm> &getAllForms() const {
    return m_loadedForms;
  }

  Texture2D getAnimationTexture(const std::string &formId,
                                const std::string &animationName);
  std::string findAnimationBaseName(const PokemonForm &form,
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
  std::map<std::string, PokemonForm> m_loadedForms;

  std::map<std::string, TrackerEntry> m_trackerData;
  bool m_trackerLoaded = false;

  std::map<std::string, Texture2D> m_textureCache;
};
