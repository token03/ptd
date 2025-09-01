#pragma once

#include <memory>
#include <vector>

#include "core/GameObject.h"
#include "factories/MobFactory.h"
#include "factories/TowerFactory.h"
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
  void LoadTestData();
  void Update(float deltaTime);
  void Draw(float deltaTime);

  const int screenWidth = 1280;
  const int screenHeight = 720;
  Font font;

  std::vector<std::shared_ptr<GameObject>> m_gameObjects;
  std::vector<std::shared_ptr<GameObject>> m_spawnQueue;

  std::shared_ptr<AssetManager> m_assetManager;
  std::shared_ptr<DataManager> m_dataManager;
  std::shared_ptr<TowerFactory> m_towerFactory;
  std::shared_ptr<MobFactory> m_mobFactory;
};
