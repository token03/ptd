#include "core/Game.h"

#include <imgui.h>

#include <filesystem>

#include "managers/AssetManager.h"
#include "managers/DataManager.h"
#include "raylib.h"
#include "rlImGui.h"
#include "scenes/GameplayScene.h"
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

  m_sceneManager = std::make_shared<SceneManager>();
  m_sceneManager->PushScene(
      std::make_shared<GameplayScene>(m_assetManager, m_dataManager));
}

void Game::Update(float deltaTime) { m_sceneManager->Update(deltaTime); }

void Game::Draw(float deltaTime) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawTextEx(font, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do ",
             {20, 20}, 32, 1, DARKGRAY);

  m_sceneManager->Draw(deltaTime);

  auto currentScene = m_sceneManager->GetCurrentScene();
  if (currentScene) {
    Debugger::GetInstance().Draw(currentScene->GetGameObjects(), deltaTime);
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
