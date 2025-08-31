#include "tools/Console.h"

#include "imgui.h"
#include "spdlog/spdlog.h"

Console::Console() : m_showWindow(false) {}

void Console::Draw() {
  if (!m_showWindow) {
    return;
  }

  if (ImGui::Begin("Console", &m_showWindow)) {
    ImGui::Text("Console output will appear here.");
    ImGui::Separator();

    ImGui::BeginChild("ConsoleOutput", ImVec2(0, -25), true);
    ImGui::Text("[INFO] Debugger initialized");
    ImGui::Text("[DEBUG] Console window opened");
    ImGui::EndChild();

    static char inputBuffer[256] = "";
    if (ImGui::InputText("Command", inputBuffer, sizeof(inputBuffer),
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
      spdlog::info("Console command: {}", inputBuffer);
      inputBuffer[0] = '\0';
    }
  }
  ImGui::End();
}
