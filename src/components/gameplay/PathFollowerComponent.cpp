#include "PathFollowerComponent.h"

#include <algorithm>
#include <memory>

#include "components/gameplay/PathComponent.h"
#include "core/GameObject.h"  // IWYU pragma: keep - required for assignRequiredComponent
#include "raymath.h"

PathFollowerComponent::PathFollowerComponent(std::weak_ptr<PathComponent> path,
                                             float speed)
    : m_path(path), m_speed(speed) {}

void PathFollowerComponent::Init() { assignRequiredComponent(m_transform, m_movable); }

void PathFollowerComponent::Update(float deltaTime) {
  auto path = m_path.lock();
  auto transform = m_transform.lock();
  auto movable = m_movable.lock();
  auto ownerGameObject = owner.lock();

  if (path && transform && movable && ownerGameObject) {
    float totalLength = path->GetTotalLength();

    if (m_distanceTraveled >= totalLength) {
      movable->SetVelocity({0.0f, 0.0f});
      ownerGameObject->Destroy();
      return;
    }

    Vector2 lastPathPosition = path->GetPointAtDistance(m_distanceTraveled);

    m_distanceTraveled += m_speed * deltaTime;
    m_distanceTraveled = std::clamp(m_distanceTraveled, 0.0f, totalLength);

    Vector2 targetPosition = path->GetPointAtDistance(m_distanceTraveled);

    Vector2 requiredVelocity = {0.0f, 0.0f};
    if (deltaTime > 0.0f) {
      Vector2 pathMovement = Vector2Subtract(targetPosition, transform->position);
      requiredVelocity = Vector2Scale(pathMovement, 1.0f / deltaTime);
    }
    movable->SetVelocity(requiredVelocity);
  }
}
