#include "components/SpriteComponent.h"
#include "core/GameObject.h"

SpriteComponent::SpriteComponent(Texture2D tex) : texture(tex) {
  sourceRect = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
}

void SpriteComponent::Init() {
  if (std::shared_ptr<GameObject> ownerPtr = owner.lock()) {
    if (!ownerPtr->HasComponent<TransformComponent>()) {
      ownerPtr->AddComponent<TransformComponent>();
    }
    m_transform = ownerPtr->GetComponentShared<TransformComponent>();
  }
}

void SpriteComponent::Draw() {
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
