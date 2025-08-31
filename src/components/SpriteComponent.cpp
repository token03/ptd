#include "components/SpriteComponent.h"
#include "core/GameObject.h" // IWYU pragma: keep - required for assignRequiredComponent

SpriteComponent::SpriteComponent(Texture2D tex) : texture(tex) {
  sourceRect = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
  origin = {(float)texture.width / 2.0f, (float)texture.height / 2.0f};
}

void SpriteComponent::Init() { assignRequiredComponent(m_transform); }

void SpriteComponent::Draw() {
  if (!isVisible)
    return;

  if (auto transform = m_transform.lock()) {
    Rectangle destRect = {transform->position.x, transform->position.y,
                          sourceRect.width * transform->scale.x,
                          sourceRect.height * transform->scale.y};

    Vector2 scaledOrigin = {origin.x * transform->scale.x,
                            origin.y * transform->scale.y};

    float rotationToApply = 0.0f;
    if (rotationMode == RotationMode::USE_TRANSFORM) {
      rotationToApply = transform->rotation;
    }

    DrawTexturePro(texture, sourceRect, destRect, scaledOrigin, rotationToApply,
                   tint);

    Rectangle actualScreenRect = {transform->position.x - scaledOrigin.x,
                                  transform->position.y - scaledOrigin.y,
                                  destRect.width, destRect.height};

    DrawRectangleLinesEx(actualScreenRect, 1.0f, GREEN);
    DrawCircleV(transform->position, 3.0f, RED);
  }
}
