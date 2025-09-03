#include "managers/DataManager.h"

#include <filesystem>
#include <glaze/glaze.hpp>
#include <memory>
#include <vector>

#include "spdlog/spdlog.h"
#include "utils/PMDUtils.h"

DataManager::DataManager() {
  const std::filesystem::path dataRoot = "data";
  const std::filesystem::path assetRoot = "assets";
  m_pmdCollabPath = assetRoot / "pmdcollab";
  m_portraitPath = m_pmdCollabPath / "portrait";

  loadSpecies((dataRoot / "species.json").string());
  loadSpeciesAlt((dataRoot / "speciesAlt.json").string());
  loadPokedex((dataRoot / "pokedex.json").string());
  loadTypeChart((dataRoot / "types.json").string());
  loadTracker((assetRoot / "pmdcollab" / "tracker.json").string());
  processTrackerData();
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

void DataManager::loadTracker(const std::string &path) {
  spdlog::debug("Loading tracker file: {}", path);
  std::string buffer;
  auto error = glz::read_file_json<glz::opts{.error_on_unknown_keys = false}>(
      m_trackerData, path, buffer);
  if (error) {
    spdlog::error("Failed to parse tracker.json: {}", glz::format_error(error, buffer));
    return;
  }
  spdlog::info("Tracker.json loaded successfully with {} root entries.",
               m_trackerData.size());
}

void DataManager::processTrackerData() {
  spdlog::info("Processing tracker entries...");
  const auto &trackerData = getTrackerData();
  for (const auto &[dex, entry] : trackerData) {
    processTrackerEntry(dex, "", entry, "", std::filesystem::path());
  }
  spdlog::info("Finished processing tracker entries. Loaded {} forms.",
               m_loadedForms.size());
}

void DataManager::processTrackerEntry(const std::string &dex,
                                      const std::string &subgroupId,
                                      const TrackerEntry &entry,
                                      const std::string &parentName,
                                      const std::filesystem::path &parentPath) {
  std::string currentFullName = PMDUtils::generateFullName(parentName, entry.name);
  std::filesystem::path currentRelativePath = parentPath / subgroupId;

  auto newForm = std::make_shared<PMDData>();
  newForm->dex = dex;
  newForm->fullId = PMDUtils::generateFullId(dex, currentRelativePath);
  newForm->fullName = currentFullName;
  newForm->formPath = currentRelativePath.string();
  newForm->spriteCredit = entry.sprite_credit;
  newForm->portraitCredit = entry.portrait_credit;

  std::filesystem::path baseSpritePath =
      m_pmdCollabPath / "sprite" / dex / currentRelativePath;
  newForm->animData = PMDUtils::parseAnimationData(baseSpritePath / "AnimData.xml");
  newForm->animFileBases = PMDUtils::findAnimationBases(baseSpritePath);

  std::filesystem::path portraitFormPath = m_portraitPath / dex / currentRelativePath;
  newForm->availablePortraits = PMDUtils::findAvailablePortraits(portraitFormPath);

  if (newForm->animData || !newForm->availablePortraits.empty()) {
    if (m_loadedForms.count(currentFullName)) {
      spdlog::warn("Duplicate form name detected, overwriting: {}", currentFullName);
    }
    m_loadedForms[currentFullName] = newForm;
  }

  for (const auto &[id, subEntry] : entry.subgroups) {
    processTrackerEntry(dex, id, subEntry, currentFullName, currentRelativePath);
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

const std::map<std::string, std::shared_ptr<PMDData>> &DataManager::getAllForms() const {
  return m_loadedForms;
}

std::shared_ptr<TypeChart> DataManager::getTypeChart() const { return m_typeChart; }

const std::map<std::string, TrackerEntry> &DataManager::getTrackerData() const {
  return m_trackerData;
}
