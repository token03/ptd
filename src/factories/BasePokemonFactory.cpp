#include "BasePokemonFactory.h"

#include "components/graphics/AnimationComponent.h"
#include "components/graphics/SpriteComponent.h"
#include "components/physics/TransformComponent.h"
#include "components/pokemon/PokedexComponent.h"
#include "components/pokemon/StatComponent.h"
#include "components/pokemon/TraitsComponent.h"
#include "data/PokedexData.h"
#include "raylib.h"
#include "spdlog/spdlog.h"

BasePokemonFactory::BasePokemonFactory(std::shared_ptr<TextureManager> assetManager,
                                       std::shared_ptr<DataManager> dataManager)
    : m_textureManager(std::move(assetManager)), m_dataManager(std::move(dataManager)) {}

std::shared_ptr<GameObject> BasePokemonFactory::CreatePokemonObject(
    const std::string &speciesName, const PokemonInstance &config,
    const std::string &initialAnimation, Vector2 position, Vector2 scale) {
  if (!m_textureManager || !m_dataManager) {
    spdlog::error("BasePokemonFactory error: A manager is not available.");
    return nullptr;
  }

  auto pokedexDataOpt = m_dataManager->getPokedexData(speciesName);
  if (!pokedexDataOpt) {
    spdlog::error("Could not find species data for: {}", speciesName);
    return nullptr;
  }
  const PokedexData &pokedexData = *pokedexDataOpt;

  const auto form = m_textureManager->getForm(speciesName);
  if (!form || !form->animData) {
    spdlog::error(
        "Could not create GameObject for species: {}. PMD form "
        "data not loaded or has no animations.",
        speciesName);
    return nullptr;
  }

  Texture2D texture = m_textureManager->getAnimationTexture(speciesName, initialAnimation);
  if (texture.id <= 0) {
    spdlog::error("Could not load initial texture for animation: {}", initialAnimation);
    return nullptr;
  }

  auto gameObject = std::make_shared<GameObject>();

  gameObject->AddComponent<TransformComponent>(position, scale);
  gameObject->AddComponent<SpriteComponent>(texture);
  gameObject->AddComponent<AnimationComponent>(m_textureManager, speciesName);
  gameObject->AddComponent<PokedexComponent>(pokedexData);
  gameObject->AddComponent<TraitsComponent>(config.nature, config.gender, config.isShiny);
  gameObject->AddComponent<StatComponent>(config.level, config.ivs, config.evs);
  gameObject->GetComponent<AnimationComponent>().Play(initialAnimation);

  return gameObject;
}

std::shared_ptr<GameObject> BasePokemonFactory::CreateRandomPokemonObject(
    const std::string &speciesName, int minLevel, int maxLevel,
    const std::string &initialAnimation, Vector2 position, Vector2 scale) {
  PokemonInstance config;
  config.level = GetRandomValue(minLevel, maxLevel);
  config.nature = static_cast<Nature>(GetRandomValue(0, 24));

  config.ivs.hp = GetRandomValue(0, 31);
  config.ivs.attack = GetRandomValue(0, 31);
  config.ivs.defense = GetRandomValue(0, 31);
  config.ivs.spAttack = GetRandomValue(0, 31);
  config.ivs.spDefense = GetRandomValue(0, 31);
  config.ivs.speed = GetRandomValue(0, 31);

  config.evs = {0, 0, 0, 0, 0, 0};

  // TODO: Determine gender based on species data (e.g., Nidoran, Tauros)
  config.gender = static_cast<Gender>(GetRandomValue(0, 1));

  // TODO: Remove this hardcoded shiny rate (1 in 4096)
  config.isShiny = (GetRandomValue(0, 4095) == 0);

  return CreatePokemonObject(speciesName, config, initialAnimation, position, scale);
}
