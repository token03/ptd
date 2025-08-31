#pragma once

#include <memory>
#include <vector>

#include "core/GameObject.h"
#include "tools/Console.h"
#include "tools/Editor.h"
#include "tools/Inspector.h"
#include "tools/Performance.h"
#include "tools/Spawner.h"
#include "tools/Visualizer.h"

class Debugger {
 public:
  static Debugger &GetInstance();

  Debugger(const Debugger &) = delete;
  void operator=(const Debugger &) = delete;

  void Initialize();
  void Shutdown();

  void Draw(const std::vector<std::shared_ptr<GameObject>> &gameObjects, float deltaTime);

 private:
  Debugger();
  ~Debugger();

  Console m_console;
  Editor m_editor;
  Inspector m_inspector;
  Performance m_performance;
  Spawner m_spawner;
  Visualizer m_visualizer;
};
