#include "managers/SceneManager.h"

#include "core/Scene.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::Update(float deltaTime) {
  if (!m_scenes.empty()) {
    m_scenes.back()->Update(deltaTime);
  }
}

void SceneManager::Draw(float deltaTime) {
  if (!m_scenes.empty()) {
    m_scenes.back()->Draw(deltaTime);
  }
}

void SceneManager::PushScene(std::shared_ptr<Scene> scene) {
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
