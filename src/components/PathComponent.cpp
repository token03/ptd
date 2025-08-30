#include "PathComponent.h"
#include <utility>

PathComponent::PathComponent(std::vector<Vector2> points,
                             std::unique_ptr<IPathStrategy> strategy)
    : m_points(std::move(points)), m_strategy(std::move(strategy)) {}

void PathComponent::Draw() {
  if (m_strategy) {
    m_strategy->Draw(m_points, pathColor);
  }
}

Vector2 PathComponent::GetPointAt(float t) const {
  if (m_strategy && !m_points.empty()) {
    return m_strategy->GetPointAt(m_points, t);
  }
  return {0.0f, 0.0f};
}
