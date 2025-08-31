#include "tools/Debugger.h"

#include "imgui.h"
#include "rlImGui.h"
#include "spdlog/spdlog.h"

Debugger &Debugger::GetInstance() {
  static Debugger instance;
  return instance;
}

Debugger::Debugger() { spdlog::info("Debugger instance created."); }

Debugger::~Debugger() { spdlog::info("Debugger instance destroyed."); }

void Debugger::Initialize() { rlImGuiSetup(true); }

void Debugger::Shutdown() { rlImGuiShutdown(); }

void Debugger::Draw(const std::vector<std::shared_ptr<GameObject>> &gameObjects,
                    float deltaTime) {
  rlImGuiBegin();

  if (ImGui::Begin("Debugger")) {
    ImGui::Checkbox("Console", &m_console.m_showWindow);
    ImGui::Checkbox("Editor", &m_editor.m_showWindow);
    ImGui::Checkbox("Inspector", &m_inspector.m_showWindow);
    ImGui::Checkbox("Performance", &m_performance.m_showWindow);
    ImGui::Checkbox("Spawner", &m_spawner.m_showWindow);
    ImGui::Checkbox("Visualizer", &m_visualizer.m_showWindow);
    ImGui::End();
  }

  m_console.Draw();
  m_editor.Draw();
  m_inspector.Draw(gameObjects);
  m_performance.Draw(deltaTime);
  m_spawner.Draw();
  m_visualizer.Draw();

  rlImGuiEnd();
}
