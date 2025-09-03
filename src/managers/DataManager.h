#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "data/PokedexData.h"
#include "data/TypeChart.h"

class DataManager {
 public:
  DataManager();

  std::optional<std::string> getDexNumber(const std::string &speciesName) const;
  std::optional<std::string> getSpeciesName(const std::string &dexStr) const;
  std::optional<PokedexData> getPokedexData(const std::string &id) const;
  std::optional<int> getIconIndex(const std::string &speciesName) const;

  std::shared_ptr<TypeChart> getTypeChart() const;

 private:
  void loadSpecies(const std::string &path);
  void loadPokedex(const std::string &path);
  void loadTypeChart(const std::string &path);
  void loadSpeciesAlt(const std::string &path);

  std::unordered_map<std::string, std::string> m_dexMap;
  std::unordered_map<std::string, std::string> m_revDexMap;
  std::unordered_map<std::string, int> m_speciesAltMap;
  std::unordered_map<std::string, PokedexData> m_pokedex;
  std::shared_ptr<TypeChart> m_typeChart;
};
