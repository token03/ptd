#pragma once

#include <memory>

class GameObject;

class Component : public std::enable_shared_from_this<Component> {
public:
  virtual ~Component() = default;

  virtual void Init() {}
  virtual void Update(float deltaTime) {}
  virtual void Draw() {}

  std::weak_ptr<GameObject> owner;

protected:
  template <typename... T>
  void assignRequiredComponent(std::weak_ptr<T> &...components);
};
