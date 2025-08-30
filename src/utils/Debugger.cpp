#include "utils/Debugger.h"
#include "components/AnimationComponent.h"
#include "components/TransformComponent.h"
#include "imgui.h"
#include "rlImGui.h"
#include "spdlog/spdlog.h"

Debugger &Debugger::GetInstance() {
  static Debugger instance;
  return instance;
}

Debugger::Debugger()
    : m_showMainWindow(true), m_showPerformanceWindow(false),
      m_showGameObjectInspector(false), m_selectedGameObjectIndex(-1) {
  spdlog::info("Debugger instance created.");
}

Debugger::~Debugger() { spdlog::info("Debugger instance destroyed."); }

void Debugger::Initialize() { rlImGuiSetup(true); }

void Debugger::Shutdown() { rlImGuiShutdown(); }

void Debugger::Draw(const std::vector<std::shared_ptr<GameObject>> &gameObjects,
                    float deltaTime) {
  rlImGuiBegin();

  if (m_showMainWindow) {
    DrawMainWindow();
  }
  if (m_showPerformanceWindow) {
    DrawPerformanceWindow(deltaTime);
  }
  if (m_showGameObjectInspector) {
    DrawGameObjectInspector(gameObjects);
  }

  rlImGuiEnd();
}

void Debugger::DrawMainWindow() {
  if (ImGui::Begin("Master Debug Window", &m_showMainWindow)) {
    ImGui::Text("Welcome to the PTD Debugger!");
    ImGui::Separator();
    ImGui::Checkbox("Show Performance Stats", &m_showPerformanceWindow);
    ImGui::Checkbox("Show GameObject Inspector", &m_showGameObjectInspector);
    ImGui::Separator();
    if (ImGui::Button("Spawn Random Bulbasaur")) {
      spdlog::warn("Spawn button clicked (functionality not implemented yet).");
    }
  }
  ImGui::End();
}

void Debugger::DrawPerformanceWindow(float deltaTime) {
  if (ImGui::Begin("Performance", &m_showPerformanceWindow)) {
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);
  }
  ImGui::End();
}

void Debugger::DrawGameObjectInspector(
    const std::vector<std::shared_ptr<GameObject>> &gameObjects) {
  if (ImGui::Begin("Game Object Inspector", &m_showGameObjectInspector)) {
    ImGui::BeginChild("ObjectList", ImVec2(ImGui::GetWindowWidth() * 0.4f, 0),
                      true);
    for (int i = 0; i < gameObjects.size(); ++i) {
      std::string objectName = "GameObject " + std::to_string(i);
      if (ImGui::Selectable(objectName.c_str(),
                            m_selectedGameObjectIndex == i)) {
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

      // if (auto transform =
      // selectedObject->GetComponent<TransformComponent>()) {
      //   if (ImGui::CollapsingHeader("Transform Component",
      //                               ImGuiTreeNodeFlags_DefaultOpen)) {
      //     ImGui::DragFloat2("Position", &transform.position.x);
      //     ImGui::DragFloat2("Scale", &transform.scale.x);
      //   }
      // }
      //
      // if (auto anim = selectedObject->GetComponent<AnimationComponent>()) {
      //   if (ImGui::CollapsingHeader("Animation Component",
      //                               ImGuiTreeNodeFlags_DefaultOpen)) {
      //     ImGui::Text("Current Animation: %s",
      //                 anim.GetCurrentAnimationName().c_str());
      //     ImGui::Text("Current Frame: %d", anim.GetCurrentFrame());
      //     ImGui::Text("Direction: %d", (int)anim.GetDirection());
      //   }
      // }

    } else {
      ImGui::Text("Select a GameObject to inspect.");
    }
    ImGui::EndChild();
  }
  ImGui::End();
}
