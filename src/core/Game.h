#pragma once

#include "core/GameObject.h"
#include <memory>
#include <vector>

class PMDLoader;
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
  std::shared_ptr<PMDLoader> m_loader;
  std::shared_ptr<PokemonFactory> m_pokemonFactory;
};
