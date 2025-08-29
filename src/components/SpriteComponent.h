#pragma once

#include "components/TransformComponent.h"
#include "core/Component.h"
#include "core/GameObject.h"
#include "raylib.h"
#include <memory>

class SpriteComponent : public Component {
public:
  Texture2D texture;
  Rectangle sourceRect;
  Vector2 origin = {0.0f, 0.0f};
  Color tint = WHITE;
  bool isVisible = true;

  explicit SpriteComponent(Texture2D tex) : texture(tex) {
    sourceRect = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
  }

  void Init() override {
    if (std::shared_ptr<GameObject> ownerPtr = owner.lock()) {
      if (!ownerPtr->HasComponent<TransformComponent>()) {
        ownerPtr->AddComponent<TransformComponent>();
      }
      m_transform = ownerPtr->GetComponentShared<TransformComponent>();
    }
  }

  void Draw() override {
    if (!isVisible)
      return;

    if (auto transform = m_transform.lock()) {
      Rectangle destRect = {transform->position.x, transform->position.y,
                            sourceRect.width * transform->scale.x,
                            sourceRect.height * transform->scale.y};

      DrawTexturePro(texture, sourceRect, destRect, origin, transform->rotation,
                     tint);
    }
  }

private:
  std::weak_ptr<TransformComponent> m_transform;
};
