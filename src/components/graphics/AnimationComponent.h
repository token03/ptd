#pragma once

#include <memory>
#include <string>

#include "components/graphics/SpriteComponent.h"
#include "core/Component.h"
#include "data/PMDData.h"

enum class Direction {
  South,
  SouthEast,
  East,
  NorthEast,
  North,
  NorthWest,
  West,
  SouthWest,
};

class TextureManager;

class AnimationComponent : public Component {
 public:
  AnimationComponent(std::shared_ptr<TextureManager> assetManager, std::string formId);
  void Init() override;
  void Update(float deltaTime) override;
  void Play(const std::string &animationName, bool reset = false);
  void Stop();
  void SetDirection(Direction direction);
  Direction GetDirection() const { return m_direction; }

 private:
  void UpdateSpriteRect();

  std::weak_ptr<TextureManager> m_assetManager;
  std::string m_formId;
  std::weak_ptr<const PMDData> m_pmdData;
  std::weak_ptr<SpriteComponent> m_sprite;

  std::string m_currentAnimation;
  std::string m_currentTextureBase;
  int m_currentFrame = 0;
  float m_frameTimer = 0.0f;
  bool m_isPlaying = false;
  Direction m_direction = Direction::South;
};
