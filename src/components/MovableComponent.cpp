#include "MovableComponent.h"

#include <cmath>

#include "core/GameObject.h"  // IWYU pragma: keep
#include "raymath.h"

void MovableComponent::Init() {
  assignRequiredComponent(m_transform);
  assignComponent(m_animation);
}

void MovableComponent::Update(float deltaTime) {
  auto transform = m_transform.lock();
  if (!transform) {
    return;
  }

  transform->position =
      Vector2Add(transform->position, Vector2Scale(velocity, deltaTime));

  auto animation = m_animation.lock();
  if (animation && Vector2LengthSqr(velocity) > 0.01f) {
    float angle = atan2f(velocity.y, velocity.x) * (180.0f / PI);
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
    animation->SetDirection(newDirection);
  }
}
