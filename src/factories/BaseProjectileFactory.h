
#pragma once

#include <memory>
#include <string>

#include "core/GameObject.h"
#include "data/PokemonInstance.h"
#include "managers/DataManager.h"
#include "managers/TextureManager.h"
#include "raylib.h"

class BaseProjectileFactory {
 public:
  BaseProjectileFactory(std::shared_ptr<TextureManager> assetManager,
                        std::shared_ptr<DataManager> dataManager);
  virtual ~BaseProjectileFactory() = default;

 protected:
  std::shared_ptr<GameObject> CreatePokemonObject(const std::string &speciesName,
                                                  const PokemonInstance &config,
                                                  const std::string &initialAnimation,
                                                  Vector2 position, Vector2 scale);

  std::shared_ptr<GameObject> CreateRandomPokemonObject(
      const std::string &speciesName, int minLevel, int maxLevel,
      const std::string &initialAnimation, Vector2 position, Vector2 scale);

  std::shared_ptr<TextureManager> m_textureManager;
  std::shared_ptr<DataManager> m_dataManager;
};
