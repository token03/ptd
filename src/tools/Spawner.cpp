#include "tools/Spawner.h"

#include "imgui.h"
#include "spdlog/spdlog.h"

Spawner::Spawner() : m_showWindow(false) {}

void Spawner::Draw() {
  if (!m_showWindow) {
    return;
  }

  if (ImGui::Begin("Spawner", &m_showWindow)) {
    ImGui::Text("GameObject spawning tools");
    ImGui::Separator();

    if (ImGui::Button("Spawn Random Bulbasaur")) {
      spdlog::warn("Spawn button clicked (functionality not implemented yet).");
    }

    ImGui::Separator();
    ImGui::Text("Spawner functionality will be expanded here.");
  }
  ImGui::End();
}
