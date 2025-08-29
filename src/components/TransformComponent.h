#pragma once

#include "core/Component.h"
#include "raylib.h"

class TransformComponent : public Component {
public:
  Vector2 position = {0.0f, 0.0f};
  Vector2 scale = {1.0f, 1.0f};
  float rotation = 0.0f;
};
