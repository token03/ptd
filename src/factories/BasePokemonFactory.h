#pragma once

#include <memory>
#include <string>

#include "core/GameObject.h"
#include "data/PokemonInstance.h"
#include "managers/AssetManager.h"
#include "managers/DataManager.h"
#include "raylib.h"

class BasePokemonFactory {
 public:
  BasePokemonFactory(std::shared_ptr<AssetManager> assetManager,
                     std::shared_ptr<DataManager> dataManager);
  virtual ~BasePokemonFactory() = default;

 protected:
  std::shared_ptr<GameObject> CreatePokemonObject(const std::string &speciesName,
                                                  const PokemonInstance &config,
                                                  const std::string &initialAnimation,
                                                  Vector2 position, Vector2 scale);

  std::shared_ptr<GameObject> CreateRandomPokemonObject(
      const std::string &speciesName, int minLevel, int maxLevel,
      const std::string &initialAnimation, Vector2 position, Vector2 scale);

  std::shared_ptr<AssetManager> m_assetManager;
  std::shared_ptr<DataManager> m_dataManager;
};
