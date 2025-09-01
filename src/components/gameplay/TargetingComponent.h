#pragma once

#include <memory>

#include "core/Component.h"
#include "core/GameObject.h"

class AnimationComponent;
class TransformComponent;

class TargetingComponent : public Component {
 public:
  TargetingComponent();
  void Init() override;
  void Update(float deltaTime) override;

 private:
  void FindTarget();
  void ValidateTarget();
  void UpdateStateAndAim();

  std::weak_ptr<GameObject> m_currentTarget;

  std::weak_ptr<AnimationComponent> m_animation;
  std::weak_ptr<TransformComponent> m_transform;

  float m_attackRange = 300.0f;
};
