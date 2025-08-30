#include "TransformComponent.h"
#include "raymath.h"
#include <cmath>

Vector2 TransformComponent::GetForward() const {
  float radians = rotation * (PI / 180.0f);
  return {cosf(radians), sinf(radians)};
}

float TransformComponent::DistanceTo(const TransformComponent &other) const {
  return Vector2Distance(position, other.position);
}

void TransformComponent::LookAt(const Vector2 &target) {
  Vector2 direction = Vector2Subtract(target, position);
  rotation = atan2f(direction.y, direction.x) * (180.0f / PI);
}

void TransformComponent::MoveTowards(const Vector2 &target, float speed,
                                     float deltaTime) {
  Vector2 direction = Vector2Subtract(target, position);
  float distance = Vector2Length(direction);

  if (distance > 0.0f) {
    Vector2 normalized = Vector2Normalize(direction);
    Vector2 movement = Vector2Scale(normalized, speed * deltaTime);

    if (Vector2Length(movement) > distance) {
      position = target;
    } else {
      position = Vector2Add(position, movement);
    }
  }
}
