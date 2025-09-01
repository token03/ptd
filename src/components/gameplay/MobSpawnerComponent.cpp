#include "MobSpawnerComponent.h"

#include "components/gameplay/PathComponent.h"
#include "components/gameplay/PathFollowerComponent.h"
#include "factories/MobFactory.h"
#include "spdlog/spdlog.h"

MobSpawnerComponent::MobSpawnerComponent(
    std::shared_ptr<MobFactory> factory, std::weak_ptr<PathComponent> path,
    std::vector<std::shared_ptr<GameObject>> &spawnQueue)
    : m_factory(factory), m_path(path), m_spawnQueue(spawnQueue) {
  m_spawnTimer = m_spawnInterval;
}

void MobSpawnerComponent::Update(float deltaTime) {
  m_spawnTimer -= deltaTime;
  if (m_spawnTimer <= 0) {
    SpawnMob();
    m_spawnTimer = m_spawnInterval;
  }
}

void MobSpawnerComponent::SpawnMob() {
  auto path = m_path.lock();
  if (!m_factory || !path) {
    spdlog::warn("MobSpawnerComponent is missing its factory or path.");
    return;
  }

  Vector2 startPosition = path->GetPointAt(0.0f);

  auto newMob =
      m_factory->CreateRandomMob("raichu", 5, 5, "Walk", startPosition, {2.5f, 2.5f});

  if (newMob) {
    newMob->AddComponent<PathFollowerComponent>(m_path, 500.0f);
    m_spawnQueue.push_back(newMob);
  }
}
