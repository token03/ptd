#include "MobFactory.h"

#include "components/gameplay/TagComponent.h"
#include "components/physics/ColliderComponent.h"
#include "components/physics/MovementComponent.h"
#include "spdlog/spdlog.h"

std::shared_ptr<GameObject> MobFactory::CreateMob(const std::string &speciesName,
                                                  const PokemonInstance &config,
                                                  const std::string &initialAnimation,
                                                  Vector2 position, Vector2 scale) {
  auto mob = CreatePokemonObject(speciesName, config, initialAnimation, position, scale);

  if (mob) {
    mob->AddComponent<MovementComponent>();
    mob->AddComponent<ColliderComponent>();
    mob->AddComponent<TagComponent>("mob");
    spdlog::debug("Created mob: {}", speciesName);
  }

  return mob;
}

std::shared_ptr<GameObject> MobFactory::CreateRandomMob(
    const std::string &speciesName, int minLevel, int maxLevel,
    const std::string &initialAnimation, Vector2 position, Vector2 scale) {
  auto mob = CreateRandomPokemonObject(speciesName, minLevel, maxLevel, initialAnimation,
                                       position, scale);

  if (mob) {
    mob->AddComponent<MovementComponent>();
    mob->AddComponent<ColliderComponent>();
    mob->AddComponent<TagComponent>("mob");
    spdlog::debug("Created random mob: {}", speciesName);
  }

  return mob;
}
