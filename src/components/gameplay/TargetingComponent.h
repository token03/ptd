#pragma once

#include <memory>
#include <vector>

#include "core/Component.h"
#include "core/GameObject.h"

class AnimationComponent;
class TransformComponent;

// TODO: Implement targeting logic (e.g., find nearest enemy, targeting priority)
class TargetingComponent : public Component {
 public:
  TargetingComponent(const std::vector<std::shared_ptr<GameObject>> &allGameObjects);
  void Init() override;
  void Update(float deltaTime) override;

 private:
  void FindTarget();
  void ValidateTarget();
  void UpdateStateAndAim();

  const std::vector<std::shared_ptr<GameObject>> &m_allGameObjects;
  std::weak_ptr<GameObject> m_currentTarget;

  std::weak_ptr<AnimationComponent> m_animation;
  std::weak_ptr<TransformComponent> m_transform;

  float m_attackRange = 300.0f;
};
