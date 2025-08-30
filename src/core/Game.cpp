#include "core/Game.h"
#include "components/AnimationComponent.h"
#include "factories/PokemonFactory.h"
#include "loaders/PMDLoader.h"
#include "loaders/PokemonDataLoader.h"
#include "raylib.h"
#include "rlImGui.h"
#include "spdlog/spdlog.h"
#include <filesystem>

Game::Game() {}
Game::~Game() {}

void Game::Load() {
  std::filesystem::path projectRoot = ".";
  m_loader = std::make_shared<PMDLoader>(projectRoot / "assets");
  auto dataLoader = std::make_shared<PokemonDataLoader>(
      (projectRoot / "data/pokedex.json").string());

  m_pokemonFactory = std::make_shared<PokemonFactory>(m_loader, dataLoader);

  LoadTestData();
}

void Game::LoadTestData() {
  PokemonInstanceConfig slowkingConfig;
  slowkingConfig.level = 50;
  slowkingConfig.nature = Nature::Modest;
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
      "bulbasaur", 5, 10, "Walk", {100.0f, 100.0f}, {2.0f, 2.0f});
  if (randomMon) {
    m_gameObjects.push_back(randomMon);
  }
}

void Game::Update(float deltaTime) {
  for (auto &go : m_gameObjects) {
    go->Update(deltaTime);
  }
}

void Game::Draw() {
  ClearBackground(RAYWHITE);
  DrawText("ptd", 20, 20, 20, DARKGRAY);

  for (auto &go : m_gameObjects) {
    go->Draw();
  }
}

int Game::Run() {
  InitWindow(screenWidth, screenHeight, "ptd");
  rlImGuiSetup(true);

  SetTargetFPS(60);

  Load();

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();
    Update(deltaTime);

    BeginDrawing();
    rlImGuiBegin();

    Draw();

    rlImGuiEnd();
    EndDrawing();
  }

  rlImGuiShutdown();
  CloseWindow();

  return 0;
}
