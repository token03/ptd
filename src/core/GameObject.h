#pragma once

#include "core/Component.h"
#include "spdlog/spdlog.h"
#include <map>
#include <memory>
#include <typeindex>

class Component;

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
    newComponent->owner = shared_from_this();

    m_components[typeid(T)] = newComponent;

    newComponent->Init();
    return *newComponent;
  }

  template <typename T> T &GetComponent() {
    auto it = m_components.find(typeid(T));
    if (it == m_components.end()) {
      spdlog::error("Attempted to get non-existent component: {}",
                    typeid(T).name());
      throw std::runtime_error("Component not found");
    }
    return *std::static_pointer_cast<T>(it->second);
  }

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
  std::map<std::type_index, std::shared_ptr<Component>> m_components;
};
