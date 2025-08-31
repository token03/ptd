#pragma once

#include "core/GameObject.h"
#include <memory>
#include <vector>

class Inspector {
public:
  Inspector();
  void Draw(const std::vector<std::shared_ptr<GameObject>> &gameObjects);

  bool m_showWindow;

private:
  int m_selectedGameObjectIndex;
};
