#include "TowerFactory.h"

#include "components/gameplay/AttackComponent.h"
#include "components/gameplay/TargetingComponent.h"
#include "components/physics/ColliderComponent.h"
#include "spdlog/spdlog.h"

std::shared_ptr<GameObject> TowerFactory::CreateTower(const std::string &speciesName,
                                                      const PokemonInstance &config,
                                                      const std::string &initialAnimation,
                                                      Vector2 position, Vector2 scale) {
  auto tower =
      CreatePokemonObject(speciesName, config, initialAnimation, position, scale);

  if (tower) {
    tower->AddComponent<AttackComponent>();
    tower->AddComponent<TargetingComponent>();
    tower->AddComponent<ColliderComponent>();
    spdlog::debug("Created tower: {}", speciesName);
  }

  return tower;
}

std::shared_ptr<GameObject> TowerFactory::CreateRandomTower(
    const std::string &speciesName, int minLevel, int maxLevel,
    const std::string &initialAnimation, Vector2 position, Vector2 scale) {
  auto tower = CreateRandomPokemonObject(speciesName, minLevel, maxLevel,
                                         initialAnimation, position, scale);

  if (tower) {
    tower->AddComponent<AttackComponent>();
    tower->AddComponent<TargetingComponent>();
    tower->AddComponent<ColliderComponent>();
    spdlog::debug("Created random tower: {}", speciesName);
  }

  return tower;
}
