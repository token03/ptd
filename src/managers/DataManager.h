#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "data/PMDData.h"
#include "data/PokedexData.h"
#include "data/TypeChart.h"

class DataManager {
 public:
  DataManager();

  std::optional<std::string> getDexNumber(const std::string &speciesName) const;
  std::optional<PokedexData> getPokedexData(const std::string &id) const;
  std::optional<int> getIconIndex(const std::string &speciesName) const;

  const std::map<std::string, std::shared_ptr<PMDData>> &getAllForms() const;
  std::shared_ptr<TypeChart> getTypeChart() const;
  const std::map<std::string, TrackerEntry> &getTrackerData() const;

 private:
  void loadSpecies(const std::string &path);
  void loadPokedex(const std::string &path);
  void loadTypeChart(const std::string &path);
  void loadSpeciesAlt(const std::string &path);
  void loadTracker(const std::string &path);
  void processTrackerData();
  void processTrackerEntry(const std::string &dex, const std::string &subgroupId,
                           const TrackerEntry &entry, const std::string &parentName,
                           const std::filesystem::path &parentPath);

  std::filesystem::path m_pmdCollabPath;
  std::filesystem::path m_portraitPath;

  std::map<std::string, std::shared_ptr<PMDData>> m_loadedForms;
  std::unordered_map<std::string, std::string> m_dexMap;
  std::unordered_map<std::string, int> m_speciesAltMap;
  std::unordered_map<std::string, PokedexData> m_pokedex;
  std::shared_ptr<TypeChart> m_typeChart;
  std::map<std::string, TrackerEntry> m_trackerData;
};
