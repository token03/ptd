#include "components/gameplay/TargetingComponent.h"

#include <cmath>

#include "components/gameplay/PathFollowerComponent.h"
#include "components/gameplay/TagComponent.h"
#include "components/graphics/AnimationComponent.h"
#include "components/physics/TransformComponent.h"
#include "core/GameObject.h"
#include "raymath.h"

TargetingComponent::TargetingComponent(
    const std::vector<std::shared_ptr<GameObject>> &allGameObjects)
    : m_allGameObjects(allGameObjects) {}

void TargetingComponent::Init() {
  assignRequiredComponent(m_transform);
  assignRequiredComponent(m_animation);
}

void TargetingComponent::Update(float deltaTime) {
  ValidateTarget();

  if (!m_currentTarget.lock()) {
    FindTarget();
  }

  UpdateStateAndAim();
}

void TargetingComponent::ValidateTarget() {
  auto target = m_currentTarget.lock();
  if (!target) return;

  auto selfTransform = m_transform.lock();
  if (!selfTransform) return;

  auto targetTransform = target->GetComponentShared<TransformComponent>();

  if (target->IsDestroyed() || !targetTransform ||
      Vector2DistanceSqr(selfTransform->position, targetTransform->position) >
          m_attackRange * m_attackRange) {
    m_currentTarget.reset();
  }
}

void TargetingComponent::FindTarget() {
  std::weak_ptr<GameObject> bestTarget;
  float maxProgress = -1.0f;

  auto selfTransform = m_transform.lock();
  if (!selfTransform) return;
  Vector2 selfPos = selfTransform->position;

  for (const auto &potentialTarget : m_allGameObjects) {
    if (potentialTarget->IsDestroyed()) continue;

    const auto tagComp = potentialTarget->GetComponentShared<TagComponent>();
    if (!tagComp || tagComp->tag != "mob") continue;

    const auto targetTransform =
        potentialTarget->GetComponentShared<TransformComponent>();
    if (!targetTransform) continue;

    if (Vector2DistanceSqr(selfPos, targetTransform->position) >
        m_attackRange * m_attackRange) {
      continue;
    }

    const auto pathFollower =
        potentialTarget->GetComponentShared<PathFollowerComponent>();
    if (pathFollower) {
      float progress = pathFollower->GetProgress();
      if (progress > maxProgress) {
        maxProgress = progress;
        bestTarget = potentialTarget;
      }
    }
  }
  m_currentTarget = bestTarget;
}

void TargetingComponent::UpdateStateAndAim() {
  auto anim = m_animation.lock();
  auto selfTransform = m_transform.lock();
  if (!anim || !selfTransform) return;

  auto target = m_currentTarget.lock();
  if (target) {
    auto targetTransform = target->GetComponentShared<TransformComponent>();
    if (!targetTransform) {
      m_currentTarget.reset();
      return;
    }

    Vector2 directionVec =
        Vector2Subtract(targetTransform->position, selfTransform->position);

    if (Vector2LengthSqr(directionVec) > 0.01f) {
      float angle = atan2f(directionVec.y, directionVec.x) * (180.0f / PI);
      if (angle < 0) {
        angle += 360.0f;
      }

      Direction newDirection = Direction::South;
      if (angle >= 337.5f || angle < 22.5f) {
        newDirection = Direction::East;
      } else if (angle >= 22.5f && angle < 67.5f) {
        newDirection = Direction::SouthEast;
      } else if (angle >= 67.5f && angle < 112.5f) {
        newDirection = Direction::South;
      } else if (angle >= 112.5f && angle < 157.5f) {
        newDirection = Direction::SouthWest;
      } else if (angle >= 157.5f && angle < 202.5f) {
        newDirection = Direction::West;
      } else if (angle >= 202.5f && angle < 247.5f) {
        newDirection = Direction::NorthWest;
      } else if (angle >= 247.5f && angle < 292.5f) {
        newDirection = Direction::North;
      } else if (angle >= 292.5f && angle < 337.5f) {
        newDirection = Direction::NorthEast;
      }
      anim->SetDirection(newDirection);
    }

    anim->Play("Attack", false);
  } else {
    anim->Play("Idle", false);
  }
}
