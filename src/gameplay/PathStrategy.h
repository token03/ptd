#pragma once

#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <vector>

class IPathStrategy {
public:
  virtual ~IPathStrategy() = default;
  virtual void Draw(const std::vector<Vector2> &points, Color color) const = 0;
  virtual Vector2 GetPointAt(const std::vector<Vector2> &points,
                             float t) const = 0;
};

class LinearPathStrategy : public IPathStrategy {
public:
  void Draw(const std::vector<Vector2> &points, Color color) const override {
    if (points.size() < 2)
      return;
    DrawLineStrip(points.data(), points.size(), color);
  }

  Vector2 GetPointAt(const std::vector<Vector2> &points,
                     float t) const override {
    if (points.size() < 2)
      return {0, 0};

    t = std::clamp(t, 0.0f, 1.0f);

    int numSegments = points.size() - 1;
    float scaledT = t * numSegments;
    int segmentIndex = std::min((int)floor(scaledT), numSegments - 1);
    float localT = scaledT - segmentIndex;

    const Vector2 &p1 = points[segmentIndex];
    const Vector2 &p2 = points[segmentIndex + 1];

    return GetSplinePointLinear(p1, p2, localT);
  }
};

class CatmullRomPathStrategy : public IPathStrategy {
public:
  void Draw(const std::vector<Vector2> &points, Color color) const override {
    if (points.size() < 4)
      return;
    DrawSplineCatmullRom(points.data(), points.size(), 2.0f, color);
  }

  Vector2 GetPointAt(const std::vector<Vector2> &points,
                     float t) const override {
    if (points.size() < 4)
      return {0, 0};

    t = std::clamp(t, 0.0f, 1.0f);

    int numSegments = points.size() - 3;
    float scaledT = t * numSegments;
    int segmentIndex = std::min((int)floor(scaledT), numSegments - 1);
    float localT = scaledT - segmentIndex;

    const Vector2 &p1 = points[segmentIndex];
    const Vector2 &p2 = points[segmentIndex + 1];
    const Vector2 &p3 = points[segmentIndex + 2];
    const Vector2 &p4 = points[segmentIndex + 3];

    return GetSplinePointCatmullRom(p1, p2, p3, p4, localT);
  }
};
