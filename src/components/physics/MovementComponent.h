#pragma once

#include <memory>

#include "components/graphics/AnimationComponent.h"
#include "components/physics/TransformComponent.h"
#include "core/Component.h"
#include "raylib.h"

class MovementComponent : public Component {
 public:
  Vector2 velocity = {0.0f, 0.0f};

  void Init() override;
  void Update(float deltaTime) override;
  void SetVelocity(Vector2 newVelocity) { velocity = newVelocity; }

 private:
  std::weak_ptr<TransformComponent> m_transform;
  std::weak_ptr<AnimationComponent> m_animation;
};
