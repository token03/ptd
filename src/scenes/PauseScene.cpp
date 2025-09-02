#include "scenes/PauseScene.h"

#include "managers/SceneManager.h"
#include "raylib.h"

PauseScene::PauseScene() {}
PauseScene::~PauseScene() {}

void PauseScene::Load() {}
void PauseScene::Unload() {}

bool PauseScene::IsOverlay() const { return true; }

void PauseScene::Update(float deltaTime) {
  Scene::Update(deltaTime);

  if (IsKeyPressed(KEY_P)) {
    if (auto sm = m_sceneManager.lock()) {
      sm->PopScene();
    }
  }
}

void PauseScene::Draw(float deltaTime) {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

  const char* text = "PAUSED";
  int fontSize = 50;
  int textWidth = MeasureText(text, fontSize);
  DrawText(text, GetScreenWidth() / 2 - textWidth / 2,
           GetScreenHeight() / 2 - fontSize / 2, fontSize, RAYWHITE);
}
