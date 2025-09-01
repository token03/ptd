#pragma once

#include "factories/BasePokemonFactory.h"

class MobFactory : public BasePokemonFactory {
 public:
  using BasePokemonFactory::BasePokemonFactory;

  std::shared_ptr<GameObject> CreateMob(const std::string &speciesName,
                                        const PokemonInstance &config,
                                        const std::string &initialAnimation,
                                        Vector2 position = {0, 0},
                                        Vector2 scale = {1, 1});

  std::shared_ptr<GameObject> CreateRandomMob(const std::string &speciesName,
                                              int minLevel, int maxLevel,
                                              const std::string &initialAnimation,
                                              Vector2 position = {0, 0},
                                              Vector2 scale = {1, 1});
};
