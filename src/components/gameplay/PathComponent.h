#pragma once

#include <vector>

#include "core/Component.h"
#include "raylib.h"

enum PathType { LINEAR, CATMULL_ROM };

class PathComponent : public Component {
 public:
  PathComponent(std::vector<Vector2> points, PathType type);

  void Draw() override;
  void DrawDebug();
  Vector2 GetPointAt(float t) const;
  Vector2 GetPointAtDistance(float distance) const;
  float GetTotalLength() const;

  Color pathColor = LIGHTGRAY;
  PathType pathType = LINEAR;

 private:
  std::vector<Vector2> m_points;
  mutable std::vector<float> m_cumulativeLengths;
  mutable float m_totalLength = 0.0f;
  mutable bool m_lengthDataCalculated = false;

  Vector2 GetPointLinear(float t) const;
  Vector2 GetPointCatmullRom(float t) const;

  void CalculateLengthData() const;
  void CalculateLengthDataLinear() const;
  Vector2 GetPointAtDistanceLinear(float distance) const;
};
