#pragma once

#include "core/GameObject.h"
#include <memory>
#include <vector>

class Debugger {
public:
  static Debugger &GetInstance();

  Debugger(const Debugger &) = delete;
  void operator=(const Debugger &) = delete;

  void Initialize();
  void Shutdown();

  void Draw(const std::vector<std::shared_ptr<GameObject>> &gameObjects,
            float deltaTime);

private:
  Debugger();
  ~Debugger();

  void DrawMainWindow();
  void DrawPerformanceWindow(float deltaTime);
  void DrawGameObjectInspector(
      const std::vector<std::shared_ptr<GameObject>> &gameObjects);

  bool m_showMainWindow;
  bool m_showPerformanceWindow;
  bool m_showGameObjectInspector;

  int m_selectedGameObjectIndex;
};
