#include "managers/DataManager.h"

#include <filesystem>
#include <glaze/glaze.hpp>

#include "spdlog/spdlog.h"

DataManager::DataManager() {
  const std::filesystem::path dataRoot = "data";
  loadSpecies((dataRoot / "species.json").string());
  loadSpeciesAlt((dataRoot / "speciesAlt.json").string());
  loadPokedex((dataRoot / "pokedex.json").string());
  loadTypeChart((dataRoot / "types.json").string());
}

void DataManager::loadSpecies(const std::string &path) {
  std::string buffer;
  auto error = glz::read_file_json(m_dexMap, path, buffer);
  if (error) {
    spdlog::error("Failed to load or parse species.json from {}: {}", path,
                  glz::format_error(error, buffer));
  } else {
    spdlog::info("Successfully loaded {} entries from species.json", m_dexMap.size());
    for (const auto &[name, dex] : m_dexMap) {
      m_revDexMap[dex] = name;
    }
  }
}

void DataManager::loadSpeciesAlt(const std::string &path) {
  std::string buffer;
  auto error = glz::read_file_json(m_speciesAltMap, path, buffer);
  if (error) {
    spdlog::warn("Could not load or parse speciesAlt.json from {}: {}", path,
                 glz::format_error(error, buffer));
  } else {
    spdlog::info("Successfully loaded {} entries from speciesAlt.json",
                 m_speciesAltMap.size());
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

std::optional<std::string> DataManager::getSpeciesName(const std::string &dexStr) const {
  auto it = m_revDexMap.find(dexStr);
  if (it != m_revDexMap.end()) {
    return it->second;
  }
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

std::optional<int> DataManager::getIconIndex(const std::string &speciesName) const {
  auto it = m_dexMap.find(speciesName);
  if (it != m_dexMap.end()) {
    try {
      return std::stoi(it->second);
    } catch (const std::invalid_argument &ia) {
      spdlog::error("Invalid dex number format for '{}': {}", speciesName, it->second);
    } catch (const std::out_of_range &oor) {
      spdlog::error("Dex number out of range for '{}': {}", speciesName, it->second);
    }
  }

  auto alt_it = m_speciesAltMap.find(speciesName);
  if (alt_it != m_speciesAltMap.end()) {
    return alt_it->second;
  }

  spdlog::warn("Icon index for species '{}' not found in any source.", speciesName);
  return std::nullopt;
}

std::shared_ptr<TypeChart> DataManager::getTypeChart() const { return m_typeChart; }
