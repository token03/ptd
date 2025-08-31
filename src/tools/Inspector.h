#pragma once

#include <memory>
#include <vector>

#include "core/GameObject.h"

class Inspector {
 public:
  Inspector();
  void Draw(const std::vector<std::shared_ptr<GameObject>> &gameObjects);

  bool m_showWindow;

 private:
  int m_selectedGameObjectIndex;
};
