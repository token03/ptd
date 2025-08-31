#include "PathComponent.h"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <utility>

PathComponent::PathComponent(std::vector<Vector2> points, PathType type)
    : m_points(std::move(points)), pathType(type) {}

void PathComponent::Draw() {
  switch (pathType) {
  case LINEAR:
    DrawSplineLinear(m_points.data(), m_points.size(), 5, pathColor);
    break;
  case CATMULL_ROM:
    DrawSplineCatmullRom(m_points.data(), m_points.size(), 5, pathColor);
    break;
  }

  for (const auto &point : m_points) {
    DrawCircleV(point, 8.0f, GREEN);
  }
}

Vector2 PathComponent::GetPointAt(float t) const {
  if (m_points.empty())
    return {0, 0};
  if (m_points.size() == 1)
    return m_points[0];

  t = std::max(0.0f, std::min(1.0f, t));

  switch (pathType) {
  case LINEAR:
    return GetPointLinear(t);
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

void PathComponent::CalculateLengthData() const {
  if (m_lengthDataCalculated)
    return;

  m_cumulativeLengths.clear();
  m_totalLength = 0.0f;

  if (m_points.size() < 2) {
    m_lengthDataCalculated = true;
    return;
  }

  m_cumulativeLengths.push_back(0.0f);

  if (pathType == LINEAR) {
    for (size_t i = 0; i < m_points.size() - 1; ++i) {
      float segmentLength = Vector2Distance(m_points[i], m_points[i + 1]);
      m_totalLength += segmentLength;
      m_cumulativeLengths.push_back(m_totalLength);
    }
  } else if (pathType == CATMULL_ROM) {
    const int samplesPerSegment = 20;
    const int totalSamples = (m_points.size() - 1) * samplesPerSegment;
    if (totalSamples <= 0) {
      m_lengthDataCalculated = true;
      return;
    }

    Vector2 prevPoint = GetPointAt(0.0f);
    for (int i = 1; i <= totalSamples; ++i) {
      float t = static_cast<float>(i) / totalSamples;
      Vector2 currentPoint = GetPointAt(t);
      float segmentLength = Vector2Distance(prevPoint, currentPoint);
      m_totalLength += segmentLength;
      m_cumulativeLengths.push_back(m_totalLength);
      prevPoint = currentPoint;
    }
  }

  m_lengthDataCalculated = true;
}

float PathComponent::GetTotalLength() const {
  CalculateLengthData();
  return m_totalLength;
}

Vector2 PathComponent::GetPointAtDistance(float distance) const {
  CalculateLengthData();

  if (m_points.empty())
    return {0, 0};
  if (distance <= 0)
    return m_points.front();
  if (distance >= m_totalLength)
    return m_points.back();

  if (pathType == LINEAR) {
    for (size_t i = 1; i < m_cumulativeLengths.size(); ++i) {
      if (distance <= m_cumulativeLengths[i]) {
        float segmentStartDistance = m_cumulativeLengths[i - 1];
        float segmentLength = m_cumulativeLengths[i] - segmentStartDistance;
        float distanceIntoSegment = distance - segmentStartDistance;
        float t =
            (segmentLength > 0) ? (distanceIntoSegment / segmentLength) : 0.0f;
        return Vector2Lerp(m_points[i - 1], m_points[i], t);
      }
    }
  } else if (pathType == CATMULL_ROM) {
    for (size_t i = 1; i < m_cumulativeLengths.size(); ++i) {
      if (distance <= m_cumulativeLengths[i]) {
        float segmentStartDistance = m_cumulativeLengths[i - 1];
        float segmentLength = m_cumulativeLengths[i] - segmentStartDistance;

        float ratio = (segmentLength > 0)
                          ? (distance - segmentStartDistance) / segmentLength
                          : 0.0f;

        const int samplesPerSegment = 20;
        const int totalSamples = (m_points.size() - 1) * samplesPerSegment;
        float t_start = static_cast<float>(i - 1) / totalSamples;
        float t_end = static_cast<float>(i) / totalSamples;

        float corrected_t = t_start + (t_end - t_start) * ratio;

        return GetPointAt(corrected_t);
      }
    }
  }

  return m_points.back();
}
