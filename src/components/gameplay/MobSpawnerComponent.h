#pragma once

#include <memory>

#include "core/Component.h"

class MobFactory;
class PathComponent;

class MobSpawnerComponent : public Component {
 public:
  MobSpawnerComponent(std::shared_ptr<MobFactory> factory,
                      std::weak_ptr<PathComponent> path);

  void Update(float deltaTime) override;

 private:
  void SpawnMob();

  std::shared_ptr<MobFactory> m_factory;
  std::weak_ptr<PathComponent> m_path;

  float m_spawnInterval = 2.0f;
  float m_spawnTimer = 0.0f;
};
