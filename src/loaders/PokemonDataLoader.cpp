#include "loaders/PokemonDataLoader.h"
#include "spdlog/spdlog.h"
#include <glaze/glaze.hpp>

PokemonDataLoader::PokemonDataLoader(const std::string &pokedexPath) {
  loadPokedex(pokedexPath);
}

void PokemonDataLoader::loadPokedex(const std::string &path) {
  glz::opts options{.error_on_unknown_keys = false};
  std::string buffer;
  auto error = glz::read_file_json(m_dexMap, path, buffer);
  if (error) {
    spdlog::error("Failed to load or parse pokedex.json from {}: {}", path,
                  glz::format_error(error, buffer));
  } else {
    spdlog::info("Successfully loaded {} entries from pokedex.json",
                 m_dexMap.size());
  }
}

std::optional<std::string>
PokemonDataLoader::getDexNumber(const std::string &speciesName) const {
  auto it = m_dexMap.find(speciesName);
  if (it != m_dexMap.end()) {
    return it->second;
  }
  spdlog::warn("Species '{}' not found in the Pokedex map.", speciesName);
  return std::nullopt;
}

std::optional<SpeciesData>
PokemonDataLoader::getSpeciesData(const std::string &formId) const {
  spdlog::warn("Returning generic sample species data for formId: {}", formId);
  return SpeciesData{
      formId, "Unknown", {PokemonType::Normal}, {50, 50, 50, 50, 50, 50}};
}
