#include "core/Game.h"
#include "components/AnimationComponent.h"
#include "raylib.h"
#include "rlImGui.h"

#include "factories/PokemonFactory.h"
#include "loaders/PMDLoader.h"

#include <filesystem>

Game::Game() {
  std::filesystem::path projectRoot = ".";
  m_loader = std::make_shared<PMDLoader>(projectRoot / "assets");
  m_pokemonFactory = std::make_shared<PokemonFactory>(m_loader);
}

Game::~Game() {}

void Game::Load() {
  m_loader->loadPokemon("0199");

  auto slowkingObject = m_pokemonFactory->CreatePokemonObject(
      "0199", "Walk", {(float)screenWidth / 2.0f, (float)screenHeight / 2.0f},
      {2.0f, 2.0f});

  if (slowkingObject) {
    slowkingObject->GetComponent<AnimationComponent>().SetDirection(
        Direction::West);
    m_gameObjects.push_back(slowkingObject);
  } else {
    spdlog::error("Failed to create Slowking GameObject!");
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
