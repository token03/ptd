#pragma once

#include "core/Component.h"
#include <iostream>
#include <map>
#include <memory>
#include <typeindex>

// The GameObject class acts as a container for components.
// It inherits from std::enable_shared_from_this so we can safely get a
// shared_ptr to it from within its own methods (e.g., to pass to components).
class GameObject : public std::enable_shared_from_this<GameObject> {
public:
  ~GameObject() = default;

  void Update(float deltaTime) {
    for (auto const &[key, val] : m_components) {
      val->Update(deltaTime);
    }
  }

  void Draw() {
    for (auto const &[key, val] : m_components) {
      val->Draw();
    }
  }

  template <typename T, typename... TArgs> T &AddComponent(TArgs &&...args) {
    if (HasComponent<T>()) {
      return GetComponent<T>();
    }

    std::shared_ptr<T> newComponent =
        std::make_shared<T>(std::forward<TArgs>(args)...);

    // Set the owner using shared_from_this(), which is safe because the
    // GameObject will be managed by a shared_ptr.
    newComponent->owner = shared_from_this();

    m_components[typeid(T)] = newComponent;

    newComponent->Init();
    return *newComponent;
  }

  // Gets a raw reference (fast, but caller must know it's valid)
  template <typename T> T &GetComponent() {
    auto it = m_components.find(typeid(T));
    if (it == m_components.end()) {
      std::cerr << "Attempted to get non-existent component: "
                << typeid(T).name() << std::endl;
      throw std::runtime_error("Component not found");
    }
    return *std::static_pointer_cast<T>(it->second);
  }

  // Gets a shared_ptr to the component (useful for other components to store)
  template <typename T> std::shared_ptr<T> GetComponentShared() {
    auto it = m_components.find(typeid(T));
    if (it == m_components.end()) {
      return nullptr;
    }
    return std::static_pointer_cast<T>(it->second);
  }

  template <typename T> bool HasComponent() const {
    return m_components.count(typeid(T));
  }

private:
  // Components are owned by the GameObject via shared_ptr.
  std::map<std::type_index, std::shared_ptr<Component>> m_components;
};
