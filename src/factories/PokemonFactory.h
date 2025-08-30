#pragma once

#include "core/GameObject.h"
#include "data/PokemonInstance.h"
#include "raylib.h"
#include <memory>
#include <string>

class PMDLoader;
class PokemonDataLoader;

class PokemonFactory {
public:
  PokemonFactory(std::shared_ptr<PMDLoader> loader,
                 std::shared_ptr<PokemonDataLoader> dataLoader);

  std::shared_ptr<GameObject>
  CreatePokemonObject(const std::string &speciesName,
                      const PokemonInstance &config,
                      const std::string &initialAnimation,
                      Vector2 position = {0, 0}, Vector2 scale = {1, 1});

  std::shared_ptr<GameObject>
  CreateRandomPokemonObject(const std::string &speciesName, int minLevel,
                            int maxLevel, const std::string &initialAnimation,
                            Vector2 position = {0, 0}, Vector2 scale = {1, 1});

private:
  std::shared_ptr<PMDLoader> m_loader;
  std::shared_ptr<PokemonDataLoader> m_dataLoader;
};
