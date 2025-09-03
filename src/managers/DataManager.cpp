#include "managers/DataManager.h"

#include <glaze/glaze.hpp>

#include "spdlog/spdlog.h"

DataManager::DataManager(const std::string &speciesPath, const std::string &pokedexPath,
                         const std::string &typeChartPath) {
  loadSpecies(speciesPath);
  loadPokedex(pokedexPath);
  loadTypeChart(typeChartPath);
}

void DataManager::loadSpecies(const std::string &path) {
  std::string buffer;
  auto error = glz::read_file_json(m_dexMap, path, buffer);
  if (error) {
    spdlog::error("Failed to load or parse species.json from {}: {}", path,
                  glz::format_error(error, buffer));
  } else {
    spdlog::info("Successfully loaded {} entries from species.json", m_dexMap.size());
  }
}

void DataManager::loadPokedex(const std::string &path) {
  std::string buffer;
  auto error = glz::read_file_json(m_pokedex, path, buffer);
  if (error) {
    spdlog::error("Failed to load or parse pokedex.json from {}: {}", path,
                  glz::format_error(error, buffer));
  } else {
    spdlog::info("Successfully loaded {} entries from pokedex.json", m_pokedex.size());
  }
}

void DataManager::loadTypeChart(const std::string &path) {
  std::vector<TypeData> rawTypeData;
  std::string buffer;
  auto error = glz::read_file_json(rawTypeData, path, buffer);

  if (error) {
    spdlog::error("Failed to load or parse types.json from {}: {}", path,
                  glz::format_error(error, buffer));
  } else {
    m_typeChart = std::make_shared<TypeChart>(rawTypeData);
    spdlog::info("Successfully loaded type chart data for {} types.", rawTypeData.size());
  }
}

std::optional<std::string> DataManager::getDexNumber(
    const std::string &speciesName) const {
  auto it = m_dexMap.find(speciesName);
  if (it != m_dexMap.end()) {
    return it->second;
  }
  spdlog::warn("Species '{}' not found in the Pokedex map.", speciesName);
  return std::nullopt;
}

std::optional<PokedexData> DataManager::getPokedexData(const std::string &id) const {
  auto it = m_pokedex.find(id);
  if (it != m_pokedex.end()) {
    return it->second;
  }
  spdlog::warn("Pokedex data for ID '{}' not found.", id);
  return std::nullopt;
}

std::shared_ptr<TypeChart> DataManager::getTypeChart() const { return m_typeChart; }
