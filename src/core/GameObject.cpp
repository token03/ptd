#include "core/GameObject.h"

#include "core/Component.h"
#include "core/Scene.h"

void GameObject::Update(float deltaTime) {
  for (auto const& [key, val] : m_components) {
    val->Update(deltaTime);
  }
}

void GameObject::Draw() {
  for (auto const& [key, val] : m_components) {
    val->Draw();
  }
}

void GameObject::Destroy() {
  if (m_isDestroyed) {
    return;
  }
  m_isDestroyed = true;
  for (auto const& [key, val] : m_components) {
    val->OnDestroy();
  }
}

void GameObject::SetScene(std::weak_ptr<Scene> scene) { m_scene = scene; }

std::shared_ptr<Scene> GameObject::GetScene() const { return m_scene.lock(); }
