#pragma once

#include "core/Component.h"
#include "raylib.h"

class TransformComponent : public Component {
 public:
  TransformComponent(Vector2 pos = {0.0f, 0.0f}, Vector2 scl = {1.0f, 1.0f})
      : position(pos), scale(scl), rotation(0.0f) {}

  Vector2 position;
  Vector2 scale;
  float rotation;

  Vector2 GetForward() const;
  float DistanceTo(const TransformComponent &other) const;
  void LookAt(const Vector2 &target);
  void MoveTowards(const Vector2 &target, float speed, float deltaTime);
};
