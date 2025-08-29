#include "GameObject.h"

GameObject::~GameObject() = default;

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

// Template method definitions (like AddComponent, GetComponent, HasComponent)
// are typically left in the header file or explicitly instantiated in the CPP.
// For simplicity and common practice with templates in C++, they are often
// kept in the header for direct compilation where they are used.
// If you wanted to move them, you would need explicit instantiations for
// every type you use with them, which is generally not recommended unless
// you have very specific reasons (e.g., reducing compile times for very
// large projects with limited template usage).
// Therefore, the template methods remain in GameObject.h as they were.
