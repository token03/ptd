#pragma once

#include "core/Component.h"
#include "raylib.h"
#include <vector>

enum PathType { LINEAR, BEZIER, CATMULL_ROM };

class PathComponent : public Component {
public:
  PathComponent(std::vector<Vector2> points, PathType type);

  void Draw() override;
  Vector2 GetPointAt(float t) const;

  Color pathColor = LIGHTGRAY;
  PathType pathType = LINEAR;

private:
  std::vector<Vector2> m_points;

  Vector2 GetPointLinear(float t) const;
  Vector2 GetPointBezier(float t) const;
  Vector2 GetPointCatmullRom(float t) const;
};
