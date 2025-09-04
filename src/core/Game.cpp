#include "core/Game.h"

#include <imgui.h>

#include "managers/DataManager.h"
#include "managers/TextureManager.h"
#include "raylib.h"
#include "rlImGui.h"
#include "scenes/GameplayScene.h"
#include "tools/Debugger.h"

Game::Game() {}
Game::~Game() {}

void Game::Load() {
  m_configManager = std::make_shared<ConfigManager>();

  InitWindow(m_configManager->getScreenWidth(), m_configManager->getScreenHeight(),
             "ptd");

  m_dataManager = std::make_shared<DataManager>();
  m_textureManager = std::make_shared<TextureManager>(m_dataManager);
  m_sceneManager = std::make_shared<SceneManager>();

  m_sceneManager->PushScene(
      std::make_shared<GameplayScene>(m_textureManager, m_dataManager));

  rlImGuiSetup(true);
  SetTargetFPS(60);

  SetTraceLogLevel(LOG_ERROR);

  font = LoadFontEx("assets/font/Truth And Ideals-Shadow.ttf", 32, 0, 256);
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
