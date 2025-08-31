#include "tools/Visualizer.h"
#include "imgui.h"

Visualizer::Visualizer() : m_showWindow(false) {}

void Visualizer::Draw() {
  if (!m_showWindow) {
    return;
  }

  if (ImGui::Begin("Visualizer", &m_showWindow)) {
    ImGui::Text("Debug visualization tools");
    ImGui::Separator();

    static bool showColliders = false;
    static bool showBounds = false;
    static bool showGrid = false;

    ImGui::Checkbox("Show Colliders", &showColliders);
    ImGui::Checkbox("Show Bounding Boxes", &showBounds);
    ImGui::Checkbox("Show Grid", &showGrid);

    ImGui::Separator();
    ImGui::Text("Visualization options will affect rendering.");
  }
  ImGui::End();
}
