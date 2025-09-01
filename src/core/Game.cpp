#include "core/Game.h"

#include <imgui.h>

#include <algorithm>
#include <filesystem>

#include "components/gameplay/MobSpawnerComponent.h"
#include "components/gameplay/PathComponent.h"
#include "components/graphics/AnimationComponent.h"
#include "entities/PokemonFactory.h"
#include "managers/AssetManager.h"
#include "managers/DataManager.h"
#include "raylib.h"
#include "rlImGui.h"
#include "spdlog/spdlog.h"
#include "tools/Debugger.h"

Game::Game() {}
Game::~Game() {}

void Game::Load() {
  std::filesystem::path projectRoot = ".";
  auto assetsPath = projectRoot / "assets";
  auto dataPath = projectRoot / "data";

  // TODO: move this to like the UI manager or something
  font = LoadFontEx("assets/font/Truth And Ideals-Shadow.ttf", 32, 0, 256);

  m_assetManager = std::make_shared<AssetManager>(assetsPath);
  m_dataManager = std::make_shared<DataManager>((dataPath / "species.json").string(),
                                                (dataPath / "types.json").string());

  m_pokemonFactory = std::make_shared<PokemonFactory>(m_assetManager, m_dataManager);

  LoadTestData();
}

void Game::Update(float deltaTime) {
  for (auto &go : m_gameObjects) {
    go->Update(deltaTime);
  }

  if (!m_spawnQueue.empty()) {
    m_gameObjects.insert(m_gameObjects.end(), m_spawnQueue.begin(), m_spawnQueue.end());
    m_spawnQueue.clear();
  }

  m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
                                     [](const std::shared_ptr<GameObject> &go) {
                                       return go->IsDestroyed();
                                     }),
                      m_gameObjects.end());
}

void Game::Draw(float deltaTime) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawTextEx(font, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do ",
             {20, 20}, 32, 1, DARKGRAY);

  for (auto &go : m_gameObjects) {
    go->Draw();
  }

  Debugger::GetInstance().Draw(m_gameObjects, deltaTime);

  EndDrawing();
}

int Game::Run() {
  InitWindow(screenWidth, screenHeight, "ptd");
  rlImGuiSetup(true);

  SetTargetFPS(60);

  Load();

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();
    Update(deltaTime);
    Draw(deltaTime);
  }

  rlImGuiShutdown();
  CloseWindow();

  return 0;
}

void Game::LoadTestData() {
  PokemonInstance slowkingConfig;
  slowkingConfig.level = 50;
  slowkingConfig.nature = Nature::MODEST;
  slowkingConfig.ivs = {25, 10, 31, 31, 31, 15};
  slowkingConfig.gender = Gender::Male;

  auto manualMon = m_pokemonFactory->CreatePokemonObject(
      "clodsire", slowkingConfig, "Walk",
      {(float)screenWidth / 2.0f, (float)screenHeight / 2.0f}, {2.0f, 2.0f});

  manualMon->GetComponent<AnimationComponent>().SetDirection(Direction::West);
  m_gameObjects.push_back(manualMon);

  auto randomMon = m_pokemonFactory->CreateRandomPokemonObject(
      "slowking", 5, 10, "Walk", {100.0f, 100.0f}, {2.5f, 2.5f});
  if (randomMon) {
    m_gameObjects.push_back(randomMon);
  }

  auto typeChart = m_dataManager->getTypeChart();
  if (typeChart) {
    float effectiveness =
        typeChart->getEffectiveness(PokemonType::FIRE, PokemonType::GRASS);
    spdlog::info("Fire against Grass effectiveness: {}", effectiveness);
  }

  auto levelObject = std::make_shared<GameObject>();

  float padding = 200.0f;
  std::vector<Vector2> pathPoints = {
      {padding, padding},
      {padding, (float)screenHeight - padding},
      {(float)screenWidth - padding, (float)screenHeight - padding},
      {(float)screenWidth - padding, padding},
      {padding, padding},
      {padding, (float)screenHeight - padding},
      {(float)screenWidth - padding, (float)screenHeight - padding}};

  auto &path =
      levelObject->AddComponent<PathComponent>(pathPoints, PathType::CATMULL_ROM);
  path.pathColor = RED;

  levelObject->AddComponent<MobSpawnerComponent>(
      m_pokemonFactory, levelObject->GetComponentShared<PathComponent>(), m_spawnQueue);

  m_gameObjects.push_back(levelObject);
}
