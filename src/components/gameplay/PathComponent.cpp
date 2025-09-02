#include "PathComponent.h"

#include <algorithm>
#include <utility>

#include "raylib.h"
#include "raymath.h"

static Vector2 Vector2CatmullRom(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3,
                                 float t) {
  float t2 = t * t;
  float t3 = t2 * t;

  float x = 0.5f * ((2.0f * p1.x) + (-p0.x + p2.x) * t +
                    (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t2 +
                    (-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * t3);

  float y = 0.5f * ((2.0f * p1.y) + (-p0.y + p2.y) * t +
                    (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t2 +
                    (-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * t3);

  return {x, y};
}

PathComponent::PathComponent(std::vector<Vector2> points, PathType type)
    : m_points(std::move(points)), pathType(type) {}

void PathComponent::Draw() {
  switch (pathType) {
    case LINEAR:
      DrawSplineLinear(m_points.data(), m_points.size(), 2, pathColor);
      break;
    case CATMULL_ROM:
      if (m_points.size() >= 4) {
        DrawSplineCatmullRom(m_points.data(), m_points.size(), 2, pathColor);
      } else {
        DrawSplineLinear(m_points.data(), m_points.size(), 2, pathColor);
      }
      break;
  }

  DrawDebug();
}

void PathComponent::DrawDebug() {
  if (m_points.empty()) return;

  const bool useLinearDrawing =
      (pathType == LINEAR) || (pathType == CATMULL_ROM && m_points.size() < 4);

  if (useLinearDrawing) {
    if (m_points.size() == 1) {
      DrawCircleV(m_points[0], 8.0f, YELLOW);
      return;
    }
    for (size_t i = 0; i < m_points.size(); ++i) {
      Color pointColor = GREEN;
      if (i == 0) {
        pointColor = YELLOW;
      } else if (i == m_points.size() - 1) {
        pointColor = BLUE;
      }
      DrawCircleV(m_points[i], 8.0f, pointColor);
    }
  } else {
    for (size_t i = 0; i < m_points.size(); ++i) {
      Color pointColor;
      if (i == 0 || i == m_points.size() - 1) {
        pointColor = ORANGE;  // Invisible control points
      } else if (i == 1) {
        pointColor = YELLOW;  // First visible point
      } else if (i == m_points.size() - 2) {
        pointColor = BLUE;  // Last visible point
      } else {
        pointColor = GREEN;  // Intermediate visible points
      }
      DrawCircleV(m_points[i], 8.0f, pointColor);
    }
  }
}

Vector2 PathComponent::GetPointAt(float t) const {
  if (m_points.empty()) return {0, 0};
  if (m_points.size() == 1) return m_points[0];

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
  if (m_points.size() < 2) return m_points[0];

  float totalLength = (m_points.size() - 1) * t;
  int segmentIndex = static_cast<int>(totalLength);
  float localT = totalLength - segmentIndex;

  if (segmentIndex >= static_cast<int>(m_points.size()) - 1) {
    return m_points.back();
  }

  Vector2 p1 = m_points[segmentIndex];
  Vector2 p2 = m_points[segmentIndex + 1];

  return Vector2Lerp(p1, p2, localT);
}

Vector2 PathComponent::GetPointCatmullRom(float t) const {
  if (m_points.size() < 4) return GetPointLinear(t);

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

  return Vector2CatmullRom(p0, p1, p2, p3, localT);
}

void PathComponent::CalculateLengthData() const {
  if (m_lengthDataCalculated) return;

  m_cumulativeLengths.clear();
  m_totalLength = 0.0f;

  if (m_points.size() < 2) {
    m_lengthDataCalculated = true;
    return;
  }

  m_cumulativeLengths.push_back(0.0f);

  if (pathType == LINEAR) {
    CalculateLengthDataLinear();
  } else if (pathType == CATMULL_ROM) {
    if (m_points.size() < 4) {
      CalculateLengthDataLinear();
    } else {
      const int samplesPerSegment = 20;
      const int numSegments = m_points.size() - 3;
      const int totalSamples = numSegments * samplesPerSegment;

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
  }
  m_lengthDataCalculated = true;
}

void PathComponent::CalculateLengthDataLinear() const {
  for (size_t i = 0; i < m_points.size() - 1; ++i) {
    float segmentLength = Vector2Distance(m_points[i], m_points[i + 1]);
    m_totalLength += segmentLength;
    m_cumulativeLengths.push_back(m_totalLength);
  }
}

float PathComponent::GetTotalLength() const {
  CalculateLengthData();
  return m_totalLength;
}

Vector2 PathComponent::GetPointAtDistance(float distance) const {
  CalculateLengthData();

  if (m_points.empty()) return {0, 0};
  if (distance <= 0)
    return (pathType == CATMULL_ROM && m_points.size() >= 4) ? m_points[1]
                                                             : m_points.front();
  if (distance >= m_totalLength)
    return (pathType == CATMULL_ROM && m_points.size() >= 4)
               ? m_points[m_points.size() - 2]
               : m_points.back();

  if (pathType == LINEAR) {
    return GetPointAtDistanceLinear(distance);
  }

  if (pathType == CATMULL_ROM) {
    if (m_points.size() < 4) {
      return GetPointAtDistanceLinear(distance);
    } else {
      for (size_t i = 1; i < m_cumulativeLengths.size(); ++i) {
        if (distance <= m_cumulativeLengths[i]) {
          float segmentStartDistance = m_cumulativeLengths[i - 1];
          float segmentLength = m_cumulativeLengths[i] - segmentStartDistance;

          float ratio = (segmentLength > 0)
                            ? (distance - segmentStartDistance) / segmentLength
                            : 0.0f;

          const int samplesPerSegment = 20;
          const int numSegments = m_points.size() - 3;
          const int totalSamples = numSegments * samplesPerSegment;

          float t_start = static_cast<float>(i - 1) / totalSamples;
          float t_end = static_cast<float>(i) / totalSamples;

          float corrected_t = t_start + (t_end - t_start) * ratio;

          return GetPointAt(corrected_t);
        }
      }
    }
  }

  return m_points.back();
}

Vector2 PathComponent::GetPointAtDistanceLinear(float distance) const {
  for (size_t i = 1; i < m_cumulativeLengths.size(); ++i) {
    if (distance <= m_cumulativeLengths[i]) {
      float segmentStartDistance = m_cumulativeLengths[i - 1];
      float segmentLength = m_cumulativeLengths[i] - segmentStartDistance;
      float distanceIntoSegment = distance - segmentStartDistance;
      float t = (segmentLength > 0) ? (distanceIntoSegment / segmentLength) : 0.0f;
      return Vector2Lerp(m_points[i - 1], m_points[i], t);
    }
  }
  return m_points.back();
}
