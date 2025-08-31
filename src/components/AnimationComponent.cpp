#include "components/AnimationComponent.h"
#include "core/GameObject.h"
#include "managers/AssetManager.h"
#include "spdlog/spdlog.h"
#include <memory>

AnimationComponent::AnimationComponent(
    std::shared_ptr<AssetManager> assetManager, std::string formId)
    : m_assetManager(std::move(assetManager)), m_formId(std::move(formId)) {}

void AnimationComponent::Init() {
  if (std::shared_ptr<GameObject> ownerPtr = owner.lock()) {
    if (!ownerPtr->HasComponent<SpriteComponent>()) {
      throw std::runtime_error("AnimationComponent requires a SpriteComponent "
                               "on the same GameObject.");
    }
    m_sprite = ownerPtr->GetComponentShared<SpriteComponent>();
  }

  if (auto assetManager = m_assetManager.lock()) {
    const auto *form = assetManager->getForm(m_formId);
    if (!form || !form->animData) {
      throw std::runtime_error(
          "AnimationComponent could not find form data for " + m_formId);
    }
    m_animData = &form->animData.value();
  } else {
    throw std::runtime_error("AssetManager is not available for "
                             "AnimationComponent initialization.");
  }
}

void AnimationComponent::Update(float deltaTime) {
  auto sprite = m_sprite.lock();
  if (!sprite || m_currentAnimation.empty() || !m_isPlaying || !m_animData)
    return;

  auto it = m_animData->animations.find(m_currentAnimation);
  if (it == m_animData->animations.end())
    return;

  const Animation &anim = it->second;
  if (anim.durations.empty())
    return;

  m_frameTimer += deltaTime;
  float frameDuration = (float)anim.durations[m_currentFrame] / 60.0f;

  if (m_frameTimer >= frameDuration) {
    m_frameTimer -= frameDuration;
    m_currentFrame = (m_currentFrame + 1) % anim.durations.size();
    UpdateSpriteRect();
  }
}

void AnimationComponent::Play(const std::string &animationName, bool reset) {
  if (m_currentAnimation == animationName && !reset)
    return;

  if (!m_animData)
    return;

  auto it = m_animData->animations.find(animationName);
  if (it == m_animData->animations.end()) {
    spdlog::error("Animation not found: {}", animationName);
    return;
  }

  if (auto assetManager = m_assetManager.lock()) {
    const PMDData *form = assetManager->getForm(m_formId);
    if (form) {
      std::string newTextureBase =
          assetManager->findAnimationBaseName(*form, animationName);
      if (newTextureBase != m_currentTextureBase) {
        Texture2D newTexture =
            assetManager->getAnimationTexture(m_formId, animationName);
        if (newTexture.id > 0) {
          if (auto sprite = m_sprite.lock()) {
            sprite->texture = newTexture;
            m_currentTextureBase = newTextureBase;
          }
        }
      }
    }
  } else {
    spdlog::error("Cannot play animation, AssetManager is not available.");
    return;
  }

  m_currentAnimation = animationName;
  m_currentFrame = 0;
  m_frameTimer = 0.0f;
  m_isPlaying = true;

  UpdateSpriteRect();
}

void AnimationComponent::Stop() { m_isPlaying = false; }

void AnimationComponent::SetDirection(Direction direction) {
  if (m_direction == direction)
    return;

  m_direction = direction;
  UpdateSpriteRect();
}

void AnimationComponent::UpdateSpriteRect() {
  auto sprite = m_sprite.lock();
  if (!sprite || m_currentAnimation.empty() || !m_animData) {
    return;
  }

  auto it = m_animData->animations.find(m_currentAnimation);
  if (it == m_animData->animations.end()) {
    return;
  }
  const Animation &anim = it->second;

  float y_offset = static_cast<int>(m_direction) * anim.frameHeight;

  sprite->sourceRect.x = m_currentFrame * anim.frameWidth;
  sprite->sourceRect.y = y_offset;
  sprite->sourceRect.width = anim.frameWidth;
  sprite->sourceRect.height = anim.frameHeight;
  sprite->origin = {anim.frameWidth / 2.0f, anim.frameHeight / 2.0f};
}
