#pragma once

#include "factories/BasePokemonFactory.h"

class TowerFactory : public BasePokemonFactory {
 public:
  TowerFactory(std::shared_ptr<AssetManager> assetManager,
               std::shared_ptr<DataManager> dataManager);

  std::shared_ptr<GameObject> CreateTower(const std::string& speciesName,
                                          const PokemonInstance& config,
                                          const std::string& initialAnimation,
                                          Vector2 position = {0, 0},
                                          Vector2 scale = {1, 1});

  std::shared_ptr<GameObject> CreateRandomTower(const std::string& speciesName,
                                                int minLevel, int maxLevel,
                                                const std::string& initialAnimation,
                                                Vector2 position = {0, 0},
                                                Vector2 scale = {1, 1});
};
