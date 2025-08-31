#pragma once

#include "core/Component.h"
#include "core/GameObject.h"
#include <memory>
#include <vector>

class PokemonFactory;
class PathComponent;

class MobSpawnerComponent : public Component {
public:
  MobSpawnerComponent(std::shared_ptr<PokemonFactory> factory,
                      std::weak_ptr<PathComponent> path,
                      std::vector<std::shared_ptr<GameObject>> &spawnQueue);

  void Update(float deltaTime) override;

private:
  void SpawnMob();

  std::shared_ptr<PokemonFactory> m_factory;
  std::weak_ptr<PathComponent> m_path;
  std::vector<std::shared_ptr<GameObject>> &m_spawnQueue;

  float m_spawnInterval = 2.0f;
  float m_spawnTimer = 0.0f;
};
