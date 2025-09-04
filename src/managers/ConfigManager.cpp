#include "managers/ConfigManager.h"

#include <glaze/glaze.hpp>
#include <memory>

#include "spdlog/spdlog.h"

ConfigManager::ConfigManager() {
  m_config = std::make_unique<ConfigData>();
  const std::filesystem::path dataRoot = "data";
  loadConfig(dataRoot / "config.json");
}

ConfigManager::~ConfigManager() = default;

void ConfigManager::loadConfig(const std::string &path) {
  std::string buffer;
  auto error = glz::read_file_json(*m_config, path, buffer);
  if (error) {
    spdlog::warn("Could not load or parse config.json from {}: {}. Using default values.",
                 path, glz::format_error(error, buffer));

    auto write_error = glz::write_file_json(*m_config, path, buffer);
    if (write_error) {
      spdlog::error("Failed to write default config file to {}.", path);
    } else {
      spdlog::info("Wrote default config file to {}.", path);
    }
  } else {
    spdlog::info("Successfully loaded config from {}", path);
  }
}

int ConfigManager::getScreenWidth() { return m_config->screenWidth; }

int ConfigManager::getScreenHeight() { return m_config->screenHeight; }
