#include "tools/Performance.h"
#include "imgui.h"

Performance::Performance() : m_showWindow(false) {}

void Performance::Draw(float deltaTime) {
  if (!m_showWindow) {
    return;
  }

  if (ImGui::Begin("Performance", &m_showWindow)) {
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);
    ImGui::Separator();

    ImGui::Text("ImGui Draw Calls: %d", ImGui::GetIO().MetricsRenderVertices);
    ImGui::Text("ImGui Windows: %d", ImGui::GetIO().MetricsActiveWindows);
  }
  ImGui::End();
}
