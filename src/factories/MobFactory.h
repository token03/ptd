#pragma once

#include "factories/BasePokemonFactory.h"

class MobFactory : public BasePokemonFactory {
 public:
  // Inherit the constructor from the base class
  using BasePokemonFactory::BasePokemonFactory;

  // Creates a mob with a specific configuration.
  std::shared_ptr<GameObject> CreateMob(const std::string &speciesName,
                                        const PokemonInstance &config,
                                        const std::string &initialAnimation,
                                        Vector2 position = {0, 0},
                                        Vector2 scale = {1, 1});

  // Creates a mob with randomized stats.
  std::shared_ptr<GameObject> CreateRandomMob(const std::string &speciesName,
                                              int minLevel, int maxLevel,
                                              const std::string &initialAnimation,
                                              Vector2 position = {0, 0},
                                              Vector2 scale = {1, 1});
};
