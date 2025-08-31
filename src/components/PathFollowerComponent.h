#pragma once

#include "MovableComponent.h"
#include "components/TransformComponent.h"
#include "core/Component.h"
#include <memory>

class PathComponent;

class PathFollowerComponent : public Component {
public:
  PathFollowerComponent(std::weak_ptr<PathComponent> path, float speed);

  void Init() override;
  void Update(float deltaTime) override;

private:
  std::weak_ptr<PathComponent> m_path;
  std::weak_ptr<TransformComponent> m_transform;
  std::weak_ptr<MovableComponent> m_movable;

  float m_speed;
  float m_distanceTraveled = 0.0f;
};
