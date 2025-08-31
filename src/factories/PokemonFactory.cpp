#include "PokemonFactory.h"

#include "components/AnimationComponent.h"
#include "components/SpeciesComponent.h"
#include "components/SpriteComponent.h"
#include "components/StatComponent.h"
#include "components/TraitsComponent.h"
#include "components/TransformComponent.h"
#include "raylib.h"
#include "spdlog/spdlog.h"

PokemonFactory::PokemonFactory(std::shared_ptr<AssetManager> assetManager,
                               std::shared_ptr<DataManager> dataManager)
    : m_assetManager(std::move(assetManager)), m_dataManager(std::move(dataManager)) {}

std::shared_ptr<GameObject> PokemonFactory::CreatePokemonObject(
    const std::string &speciesName, const PokemonInstance &config,
    const std::string &initialAnimation, Vector2 position, Vector2 scale) {
  if (!m_assetManager || !m_dataManager) {
    spdlog::error("PokemonFactory error: A manager is not available.");
    return nullptr;
  }

  auto dexNumOpt = m_dataManager->getDexNumber(speciesName);
  if (!dexNumOpt) {
    spdlog::error("Could not find dex number for species '{}'.", speciesName);
    return nullptr;
  }
  const std::string &dexNumber = *dexNumOpt;

  auto speciesDataOpt = m_dataManager->getSpeciesData(dexNumber);
  if (!speciesDataOpt) {
    spdlog::error("Could not find species data for dex number: {}", dexNumber);
    return nullptr;
  }
  const SpeciesData &speciesData = *speciesDataOpt;

  m_assetManager->loadPokemonSpriteData(dexNumber);
  const auto form = m_assetManager->getForm(dexNumber);
  if (!form || !form->animData) {
    spdlog::error(
        "Could not create GameObject for dex number: {}. PMD form "
        "data not loaded or has no animations.",
        dexNumber);
    return nullptr;
  }

  Texture2D texture = m_assetManager->getAnimationTexture(dexNumber, initialAnimation);
  if (texture.id <= 0) {
    spdlog::error("Could not load initial texture for animation: {}", initialAnimation);
    return nullptr;
  }

  auto gameObject = std::make_shared<GameObject>();

  gameObject->AddComponent<TransformComponent>(position, scale);
  gameObject->AddComponent<SpriteComponent>(texture);
  gameObject->AddComponent<AnimationComponent>(m_assetManager, dexNumber);
  gameObject->AddComponent<SpeciesComponent>(speciesData);
  gameObject->AddComponent<TraitsComponent>(config.nature, config.gender, config.isShiny);
  gameObject->AddComponent<StatComponent>(config.level, config.ivs, config.evs);
  gameObject->GetComponent<AnimationComponent>().Play(initialAnimation);

  return gameObject;
}

std::shared_ptr<GameObject> PokemonFactory::CreateRandomPokemonObject(
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
