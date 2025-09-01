#pragma once

#include <map>
#include <memory>
#include <typeindex>

#include "core/Component.h"
#include "spdlog/spdlog.h"

class Scene;

class GameObject : public std::enable_shared_from_this<GameObject> {
 public:
  ~GameObject() = default;

  void Update(float deltaTime);
  void Draw();

  void Destroy();
  bool IsDestroyed() const { return m_isDestroyed; }

  void SetScene(std::weak_ptr<Scene> scene);
  std::shared_ptr<Scene> GetScene() const;

  template <typename T, typename... TArgs>
  T& AddComponent(TArgs&&... args) {
    if (HasComponent<T>()) {
      return GetComponent<T>();
    }

    std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<TArgs>(args)...);
    newComponent->owner = weak_from_this();

    m_components[typeid(T)] = newComponent;

    newComponent->Init();
    return *newComponent;
  }

  template <typename T>
  T& GetComponent() {
    auto it = m_components.find(typeid(T));
    if (it == m_components.end()) {
      spdlog::error("Attempted to get non-existent component: {}", typeid(T).name());
      throw std::runtime_error("Component not found");
    }
    return *std::static_pointer_cast<T>(it->second);
  }

  template <typename T>
  std::shared_ptr<T> GetComponentShared() {
    auto it = m_components.find(typeid(T));
    if (it == m_components.end()) {
      return nullptr;
    }
    return std::static_pointer_cast<T>(it->second);
  }

  template <typename T>
  bool HasComponent() const {
    return m_components.count(typeid(T));
  }

 private:
  bool m_isDestroyed = false;
  std::map<std::type_index, std::shared_ptr<Component>> m_components;
  std::weak_ptr<Scene> m_scene;
};

#include "core/Component.inl"  // IWYU pragma: keep
