#include "tools/Editor.h"

#include "imgui.h"
#include "spdlog/spdlog.h"

Editor::Editor() : m_showWindow(false) {}

void Editor::Draw() {
  if (!m_showWindow) {
    return;
  }

  if (ImGui::Begin("Editor", &m_showWindow)) {
    ImGui::Text("Level/Scene editor tools will go here.");
    ImGui::Separator();

    if (ImGui::Button("Save Scene")) {
      spdlog::info("Save scene requested");
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Scene")) {
      spdlog::info("Load scene requested");
    }

    ImGui::Separator();
    ImGui::Text("Editor functionality not yet implemented.");
  }
  ImGui::End();
}
