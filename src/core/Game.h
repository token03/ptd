#pragma once

#include <memory>

#include "managers/SceneManager.h"
#include "raylib.h"

class AssetManager;
class DataManager;

class Game {
 public:
  Game();
  ~Game();

  int Run();

 private:
  void Load();
  void Update(float deltaTime);
  void Draw(float deltaTime);

  const int screenWidth = 1280;
  const int screenHeight = 720;
  Font font;

  std::shared_ptr<AssetManager> m_assetManager;
  std::shared_ptr<DataManager> m_dataManager;
  std::unique_ptr<SceneManager> m_sceneManager;
};
