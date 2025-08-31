#pragma once

#include "core/GameObject.h"
#include "data/PokemonInstance.h"
#include "managers/AssetManager.h"
#include "managers/DataManager.h"
#include "raylib.h"
#include <memory>
#include <string>

class PMDLoader;
class PokemonDataLoader;

class PokemonFactory {
public:
  PokemonFactory(std::shared_ptr<AssetManager> loader,
                 std::shared_ptr<DataManager> dataLoader);

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
  std::shared_ptr<AssetManager> m_assetManager;
  std::shared_ptr<DataManager> m_dataManager;
};
