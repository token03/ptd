#pragma once

#include "data/SpeciesData.h"
#include "data/TypeChart.h"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

class PokemonDataLoader {
public:
  PokemonDataLoader(const std::string &pokedexPath,
                    const std::string &typeChartPath);

  std::optional<std::string> getDexNumber(const std::string &speciesName) const;
  std::optional<SpeciesData> getSpeciesData(const std::string &formId) const;

  std::shared_ptr<TypeChart> getTypeChart() const;

private:
  void loadPokedex(const std::string &path);
  void loadTypeChart(const std::string &path);

  std::unordered_map<std::string, std::string> m_dexMap;
  std::shared_ptr<TypeChart> m_typeChart;
};
