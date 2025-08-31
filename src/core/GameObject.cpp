#include "GameObject.h"
#include "Component.h"

void GameObject::Update(float deltaTime) {
  for (auto const &[key, val] : m_components) {
    val->Update(deltaTime);
  }
}

void GameObject::Draw() {
  for (auto const &[key, val] : m_components) {
    val->Draw();
  }
}

void GameObject::Destroy() {
  if (m_isDestroyed) {
    return;
  }
  m_isDestroyed = true;
  for (auto const &[key, val] : m_components) {
    val->OnDestroy();
  }
}
