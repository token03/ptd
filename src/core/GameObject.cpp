#include "GameObject.h"

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
