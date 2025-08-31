#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "data/SpeciesData.h"
#include "data/TypeChart.h"

class DataManager {
 public:
  DataManager(const std::string &speciesPath, const std::string &typeChartPath);

  std::optional<std::string> getDexNumber(const std::string &speciesName) const;
  std::optional<SpeciesData> getSpeciesData(const std::string &formId) const;

  std::shared_ptr<TypeChart> getTypeChart() const;

 private:
  void loadSpecies(const std::string &path);
  void loadTypeChart(const std::string &path);

  std::unordered_map<std::string, std::string> m_dexMap;
  std::shared_ptr<TypeChart> m_typeChart;
};
