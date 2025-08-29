#pragma once

#include "SpriteComponent.h"
#include "core/Component.h"
#include "utils/PMDData.h"
#include <memory>
#include <string>

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

class PMDLoader;

class AnimationComponent : public Component {
public:
  AnimationComponent(PMDLoader *loader, std::string formId);
  void Init() override;
  void Update(float deltaTime) override;
  void Play(const std::string &animationName, bool reset = false);
  void Stop();
  void SetDirection(Direction direction);
  Direction GetDirection() const { return m_direction; }

private:
  void UpdateSpriteRect();

  PMDLoader *m_loader;
  std::string m_formId;
  const AnimationData *m_animData = nullptr;
  std::weak_ptr<SpriteComponent> m_sprite;

  std::string m_currentAnimation;
  std::string m_currentTextureBase;
  int m_currentFrame = 0;
  float m_frameTimer = 0.0f;
  bool m_isPlaying = false;
  Direction m_direction = Direction::South;
};
