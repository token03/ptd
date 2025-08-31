#include "core/Game.h"
#include "components/AnimationComponent.h"
#include "factories/PokemonFactory.h"
#include "managers/AssetManager.h"
#include "managers/DataManager.h"
#include "raylib.h"
#include "rlImGui.h"
#include "spdlog/spdlog.h"
#include "tools/Debugger.h"
#include <filesystem>
#include <imgui.h>

Game::Game() {}
Game::~Game() {}

void Game::Load() {
  std::filesystem::path projectRoot = ".";
  auto assetsPath = projectRoot / "assets";
  auto dataPath = projectRoot / "data";

  m_assetManager = std::make_shared<AssetManager>(assetsPath);
  m_dataManager = std::make_shared<DataManager>(
      (dataPath / "pokedex.json").string(), (dataPath / "types.json").string());

  m_pokemonFactory =
      std::make_shared<PokemonFactory>(m_assetManager, m_dataManager);

  LoadTestData();
}

void Game::Update(float deltaTime) {
  for (auto &go : m_gameObjects) {
    go->Update(deltaTime);
  }
}

void Game::Draw(float deltaTime) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawText("ptd", 20, 20, 20, DARKGRAY);

  Debugger::GetInstance().Draw(m_gameObjects, deltaTime);

  for (auto &go : m_gameObjects) {
    go->Draw();
  }

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

  auto slowkingObject = m_pokemonFactory->CreatePokemonObject(
      "slowking", slowkingConfig, "Walk",
      {(float)screenWidth / 2.0f, (float)screenHeight / 2.0f}, {2.0f, 2.0f});

  if (slowkingObject) {
    slowkingObject->GetComponent<AnimationComponent>().SetDirection(
        Direction::West);
    m_gameObjects.push_back(slowkingObject);
  } else {
    spdlog::error("Failed to create Slowking GameObject!");
  }

  auto randomMon = m_pokemonFactory->CreateRandomPokemonObject(
      "raichu", 5, 10, "Walk", {100.0f, 100.0f}, {2.5f, 2.5f});
  if (randomMon) {
    m_gameObjects.push_back(randomMon);
  }

  auto typeChart = m_dataManager->getTypeChart();
  if (typeChart) {
    float effectiveness =
        typeChart->getEffectiveness(PokemonType::FIRE, PokemonType::GRASS);
    spdlog::info("Fire against Grass effectiveness: {}", effectiveness);

    effectiveness =
        typeChart->getEffectiveness(PokemonType::WATER, PokemonType::FIRE);
    spdlog::info("Water against Fire effectiveness: {}", effectiveness);

    effectiveness =
        typeChart->getEffectiveness(PokemonType::ELECTRIC, PokemonType::GROUND);
    spdlog::info("Electric against Ground effectiveness: {}", effectiveness);
  }
}
