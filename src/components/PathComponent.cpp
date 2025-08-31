#include "PathComponent.h"
#include "raylib.h"
#include <utility>

PathComponent::PathComponent(std::vector<Vector2> points, PathType type)
    : m_points(std::move(points)), pathType(type) {}

void PathComponent::Draw() {
  switch (pathType) {
  case LINEAR:
    DrawSplineLinear(m_points.data(), m_points.size(), 5, pathColor);
    break;
  case BEZIER:
    DrawSplineBezierQuadratic(m_points.data(), m_points.size(), 5, pathColor);
    break;
  case CATMULL_ROM:
    DrawSplineCatmullRom(m_points.data(), m_points.size(), 5, pathColor);
    break;
  }
}

Vector2 PathComponent::GetPointAt(float t) const {
  if (m_points.empty())
    return {0, 0};
  if (m_points.size() == 1)
    return m_points[0];

  // Clamp t to [0, 1]
  t = std::max(0.0f, std::min(1.0f, t));

  switch (pathType) {
  case LINEAR:
    return GetPointLinear(t);
  case BEZIER:
    return GetPointBezier(t);
  case CATMULL_ROM:
    return GetPointCatmullRom(t);
  default:
    return m_points[0];
  }
}

Vector2 PathComponent::GetPointLinear(float t) const {
  if (m_points.size() < 2)
    return m_points[0];

  float totalLength = (m_points.size() - 1) * t;
  int segmentIndex = static_cast<int>(totalLength);
  float localT = totalLength - segmentIndex;

  if (segmentIndex >= static_cast<int>(m_points.size()) - 1) {
    return m_points.back();
  }

  Vector2 p1 = m_points[segmentIndex];
  Vector2 p2 = m_points[segmentIndex + 1];

  return {p1.x + (p2.x - p1.x) * localT, p1.y + (p2.y - p1.y) * localT};
}

Vector2 PathComponent::GetPointBezier(float t) const {
  if (m_points.size() < 3)
    return GetPointLinear(t);

  // For quadratic Bezier, we need groups of 3 points
  int numSegments = (m_points.size() - 1) / 2;
  if (numSegments == 0)
    return m_points[0];

  float segmentT = t * numSegments;
  int segmentIndex = static_cast<int>(segmentT);
  float localT = segmentT - segmentIndex;

  if (segmentIndex >= numSegments) {
    segmentIndex = numSegments - 1;
    localT = 1.0f;
  }

  int startIdx = segmentIndex * 2;
  if (startIdx + 2 >= static_cast<int>(m_points.size())) {
    return m_points.back();
  }

  Vector2 p0 = m_points[startIdx];
  Vector2 p1 = m_points[startIdx + 1];
  Vector2 p2 = m_points[startIdx + 2];

  float u = 1.0f - localT;
  float tt = localT * localT;
  float uu = u * u;

  return {uu * p0.x + 2 * u * localT * p1.x + tt * p2.x,
          uu * p0.y + 2 * u * localT * p1.y + tt * p2.y};
}

Vector2 PathComponent::GetPointCatmullRom(float t) const {
  if (m_points.size() < 4)
    return GetPointLinear(t);

  int numSegments = m_points.size() - 3;
  float segmentT = t * numSegments;
  int segmentIndex = static_cast<int>(segmentT);
  float localT = segmentT - segmentIndex;

  if (segmentIndex >= numSegments) {
    segmentIndex = numSegments - 1;
    localT = 1.0f;
  }

  Vector2 p0 = m_points[segmentIndex];
  Vector2 p1 = m_points[segmentIndex + 1];
  Vector2 p2 = m_points[segmentIndex + 2];
  Vector2 p3 = m_points[segmentIndex + 3];

  float t2 = localT * localT;
  float t3 = t2 * localT;

  return {0.5f * ((2.0f * p1.x) + (-p0.x + p2.x) * localT +
                  (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t2 +
                  (-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * t3),
          0.5f * ((2.0f * p1.y) + (-p0.y + p2.y) * localT +
                  (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t2 +
                  (-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * t3)};
}
