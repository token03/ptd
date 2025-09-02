#include "scenes/GameplayScene.h"

#include "components/gameplay/MobSpawnerComponent.h"
#include "components/gameplay/PathComponent.h"
#include "components/graphics/AnimationComponent.h"
#include "core/GameObject.h"
#include "factories/MobFactory.h"
#include "factories/TowerFactory.h"
#include "managers/AssetManager.h"
#include "managers/DataManager.h"
#include "managers/SceneManager.h"
#include "scenes/PauseScene.h"
#include "spdlog/spdlog.h"

GameplayScene::GameplayScene(std::shared_ptr<AssetManager> assetManager,
                             std::shared_ptr<DataManager> dataManager)
    : m_assetManager(std::move(assetManager)), m_dataManager(std::move(dataManager)) {}

GameplayScene::~GameplayScene() {}

void GameplayScene::Load() {
  m_towerFactory = std::make_shared<TowerFactory>(m_assetManager, m_dataManager);
  m_mobFactory = std::make_shared<MobFactory>(m_assetManager, m_dataManager);

  m_backgroundTexture = m_assetManager->getBackgroundTexture("bg");
  if (m_backgroundTexture.id > 0) {
    m_backgroundLoaded = true;
  } else {
    spdlog::warn("Failed to load background texture 'bg.png'.");
  }

  LoadTestData();
  spdlog::info("GameplayScene loaded.");
}

void GameplayScene::Unload() {
  m_gameObjects.clear();
  m_spawnQueue.clear();
  m_backgroundLoaded = false;
  spdlog::info("GameplayScene unloaded.");
}

void GameplayScene::Update(float deltaTime) {
  if (IsKeyPressed(KEY_P)) {
    if (auto sm = m_sceneManager.lock()) {
      sm->PushScene(std::make_shared<PauseScene>());
      return;
    }
  }

  Scene::Update(deltaTime);
}

void GameplayScene::Draw(float deltaTime) {
  DrawBackground();

  Scene::Draw(deltaTime);
}

void GameplayScene::DrawBackground() const {
  if (!m_backgroundLoaded) return;

  auto tw = static_cast<float>(m_backgroundTexture.width);
  auto th = static_cast<float>(m_backgroundTexture.height);

  auto screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
  auto texAspect = tw / th;

  auto srcW = (texAspect > screenAspect) ? th * screenAspect : tw;
  auto srcH = (texAspect > screenAspect) ? th : tw / screenAspect;
  auto srcX = (tw - srcW) * 0.5f;
  auto srcY = (th - srcH) * 0.5f;

  Rectangle sourceRec = {srcX, srcY, srcW, srcH};
  Rectangle destRec = {0.0f, 0.0f, static_cast<float>(screenWidth),
                       static_cast<float>(screenHeight)};

  DrawTexturePro(m_backgroundTexture, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
}

void GameplayScene::LoadTestData() {
  PokemonInstance slowkingConfig;
  slowkingConfig.level = 50;
  slowkingConfig.nature = Nature::MODEST;
  slowkingConfig.ivs = {25, 10, 31, 31, 31, 15};
  slowkingConfig.gender = Gender::Male;

  auto manualMon = m_towerFactory->CreateTower(
      "clodsire", slowkingConfig, "Idle",
      {(float)screenWidth / 2.0f, (float)screenHeight / 2.0f}, {2.0f, 2.0f});

  manualMon->GetComponent<AnimationComponent>().SetDirection(Direction::West);
  AddGameObject(manualMon);

  auto randomMon = m_towerFactory->CreateRandomTower("slowking", 5, 10, "Idle",
                                                     {100.0f, 100.0f}, {2.5f, 2.5f});
  if (randomMon) {
    AddGameObject(randomMon);
  }

  auto typeChart = m_dataManager->getTypeChart();
  if (typeChart) {
    float effectiveness =
        typeChart->getEffectiveness(PokemonType::FIRE, PokemonType::GRASS);
    spdlog::info("Fire against Grass effectiveness: {}", effectiveness);
  }

  auto levelObject = CreateGameObject();

  float padding = 200.0f;
  std::vector<Vector2> pathPoints = {
      {padding, padding},
      {padding, (float)screenHeight - padding},
      {(float)screenWidth - padding, (float)screenHeight - padding},
      {(float)screenWidth - padding, padding},
      {padding, padding},
      {padding, (float)screenHeight - padding},
      {(float)screenWidth - padding, (float)screenHeight - padding}};

  auto& path =
      levelObject->AddComponent<PathComponent>(pathPoints, PathType::CATMULL_ROM);
  path.pathColor = RED;

  levelObject->AddComponent<MobSpawnerComponent>(
      m_mobFactory, levelObject->GetComponentShared<PathComponent>());
}
