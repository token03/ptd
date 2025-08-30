#pragma once

#include "core/Component.h"
#include "gameplay/PathStrategy.h"
#include "raylib.h"
#include <memory>
#include <vector>

class PathComponent : public Component {
public:
  PathComponent(std::vector<Vector2> points,
                std::unique_ptr<IPathStrategy> strategy);

  void Draw() override;
  Vector2 GetPointAt(float t) const;

  Color pathColor = LIGHTGRAY;

private:
  std::vector<Vector2> m_points;
  std::unique_ptr<IPathStrategy> m_strategy;
};
