#include "tools/Inspector.h"

#include <string>

#include "imgui.h"

Inspector::Inspector() : m_showWindow(false), m_selectedGameObjectIndex(-1) {}

void Inspector::Draw(const std::vector<std::shared_ptr<GameObject>> &gameObjects) {
  if (!m_showWindow) {
    return;
  }

  if (ImGui::Begin("Inspector", &m_showWindow)) {
    ImGui::BeginChild("ObjectList", ImVec2(ImGui::GetWindowWidth() * 0.4f, 0), true);
    for (int i = 0; i < gameObjects.size(); ++i) {
      std::string objectName = "GameObject " + std::to_string(i);
      if (ImGui::Selectable(objectName.c_str(), m_selectedGameObjectIndex == i)) {
        m_selectedGameObjectIndex = i;
      }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("ObjectDetails", ImVec2(0, 0), true);
    if (m_selectedGameObjectIndex >= 0 &&
        m_selectedGameObjectIndex < gameObjects.size()) {
      auto selectedObject = gameObjects[m_selectedGameObjectIndex];

      ImGui::Text("Inspecting GameObject %d", m_selectedGameObjectIndex);
      ImGui::Separator();

      ImGui::Text("Object details will be expanded here.");

    } else {
      ImGui::Text("Select a GameObject to inspect.");
    }
    ImGui::EndChild();
  }
  ImGui::End();
}
