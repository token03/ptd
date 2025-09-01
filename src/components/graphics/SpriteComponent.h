#pragma once

#include <memory>

#include "components/physics/TransformComponent.h"
#include "core/Component.h"
#include "raylib.h"

enum class RotationMode {
  IGNORE,
  USE_TRANSFORM,
};

class SpriteComponent : public Component {
 public:
  Texture2D texture;
  Rectangle sourceRect;
  Vector2 origin = {0.0f, 0.0f};
  Color tint = WHITE;
  bool isVisible = true;
  RotationMode rotationMode = RotationMode::IGNORE;

  explicit SpriteComponent(Texture2D tex);

  void Init() override;
  void Draw() override;

 private:
  std::weak_ptr<TransformComponent> m_transform;
};
