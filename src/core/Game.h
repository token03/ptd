#pragma once

#include "core/GameObject.h"
#include <memory>
#include <vector>

class AssetManager;
class DataManager;
class PokemonFactory;

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

  std::vector<std::shared_ptr<GameObject>> m_gameObjects;
  std::vector<std::shared_ptr<GameObject>> m_newGameObjects;

  std::shared_ptr<AssetManager> m_assetManager;
  std::shared_ptr<DataManager> m_dataManager;
  std::shared_ptr<PokemonFactory> m_pokemonFactory;
};
