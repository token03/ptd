#pragma once

#include "data/PokemonData.h"
#include <optional>
#include <string>
#include <unordered_map>

class PokemonDataLoader {
public:
  PokemonDataLoader(const std::string &pokedexPath);

  std::optional<std::string> getDexNumber(const std::string &speciesName) const;
  std::optional<SpeciesData> getSpeciesData(const std::string &formId) const;

private:
  void loadPokedex(const std::string &path);

  std::unordered_map<std::string, std::string> m_dexMap;
};
