#include "PathFollowerComponent.h"
#include "components/PathComponent.h"
#include "core/GameObject.h"
#include <algorithm>
#include <memory>

PathFollowerComponent::PathFollowerComponent(std::weak_ptr<PathComponent> path,
                                             float speed)
    : m_path(path), m_speed(speed) {}

void PathFollowerComponent::Init() {
  if (std::shared_ptr<GameObject> ownerPtr = owner.lock()) {
    m_transform = ownerPtr->GetComponentShared<TransformComponent>();
  }
}

void PathFollowerComponent::Update(float deltaTime) {
  auto path = m_path.lock();
  auto transform = m_transform.lock();

  if (path && transform) {
    m_progress += m_speed * deltaTime;
    m_progress = std::clamp(m_progress, 0.0f, 1.0f);

    transform->position = path->GetPointAt(m_progress);
  }
}
