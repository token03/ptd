#pragma once

#include <memory>

#include "components/AnimationComponent.h"
#include "components/TransformComponent.h"
#include "core/Component.h"
#include "raylib.h"

class MovableComponent : public Component {
 public:
  Vector2 velocity = {0.0f, 0.0f};

  void Init() override;
  void Update(float deltaTime) override;
  void SetVelocity(Vector2 newVelocity) { velocity = newVelocity; }

 private:
  std::weak_ptr<TransformComponent> m_transform;
  std::weak_ptr<AnimationComponent> m_animation;
};
