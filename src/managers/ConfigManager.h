#pragma once

#include <memory>
#include <string>

#include "data/ConfigData.h"
#include "raylib.h"

class ConfigManager {
 public:
  ConfigManager();
  ~ConfigManager();

  int getScreenWidth();
  int getScreenHeight();

 private:
  void loadConfig(const std::string &path);
  std::unique_ptr<ConfigData> m_config;
};
