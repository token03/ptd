#include "managers/SceneManager.h"

#include <algorithm>

#include "core/Scene.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::Update(float deltaTime) {
  if (!m_scenes.empty()) {
    m_scenes.back()->Update(deltaTime);
  }
}

void SceneManager::Draw(float deltaTime) {
  if (m_scenes.empty()) return;

  auto it = std::find_if(
      m_scenes.rbegin(), m_scenes.rend(),
      [](const std::shared_ptr<Scene>& scene) { return !scene->IsOverlay(); });

  auto start_it = (it == m_scenes.rend()) ? m_scenes.begin() : (it.base() - 1);

  for (auto i = start_it; i != m_scenes.end(); ++i) {
    (*i)->Draw(deltaTime);
  }
}

void SceneManager::PushScene(std::shared_ptr<Scene> scene) {
  scene->SetSceneManager(shared_from_this());
  m_scenes.push_back(scene);
  m_scenes.back()->Load();
}

void SceneManager::PopScene() {
  if (!m_scenes.empty()) {
    m_scenes.back()->Unload();
    m_scenes.pop_back();
  }
}

void SceneManager::ChangeScene(std::shared_ptr<Scene> scene) {
  PopScene();
  PushScene(scene);
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene() const {
  if (m_scenes.empty()) {
    return nullptr;
  }
  return m_scenes.back();
}

bool SceneManager::IsEmpty() const { return m_scenes.empty(); }
