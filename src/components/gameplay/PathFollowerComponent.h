#pragma once

#include <memory>

#include "components//physics/MovementComponent.h"
#include "components/physics/TransformComponent.h"
#include "core/Component.h"

class PathComponent;

class PathFollowerComponent : public Component {
 public:
  PathFollowerComponent(std::weak_ptr<PathComponent> path, float speed);

  void Init() override;
  void Update(float deltaTime) override;
  float GetProgress() const;

 private:
  std::weak_ptr<PathComponent> m_path;
  std::weak_ptr<TransformComponent> m_transform;
  std::weak_ptr<MovementComponent> m_movable;

  float m_speed;
  float m_distanceTraveled = 0.0f;
};
