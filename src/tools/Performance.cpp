#include "tools/Performance.h"
#include "imgui.h"
#include <cstdio>
#include <fstream>
#include <raylib.h>
#include <string>

Performance::Performance() : m_showWindow(false) {}

void Performance::Draw(float deltaTime) {
  if (!m_showWindow) {
    return;
  }

  if (ImGui::Begin("Performance", &m_showWindow)) {
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);
    ImGui::Text("Target FPS: %d", GetFPS());
    ImGui::Separator();

    static size_t memoryUsage = 0;
    static int frameCounter = 0;
    if (frameCounter++ % 60 == 0) {
      std::ifstream file("/proc/self/status");
      std::string line;
      while (std::getline(file, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
          sscanf(line.c_str(), "VmRSS: %zu kB", &memoryUsage);
          break;
        }
      }
    }
    ImGui::Text("Memory Usage: %.2f MB", memoryUsage / 1024.0f);
    ImGui::Separator();

    ImGui::Text("Monitor: %dx%d", GetMonitorWidth(GetCurrentMonitor()),
                GetMonitorHeight(GetCurrentMonitor()));
    ImGui::Text("Screen: %dx%d", GetScreenWidth(), GetScreenHeight());
    ImGui::Text("Time: %.2f s", GetTime());
  }
  ImGui::End();
}
