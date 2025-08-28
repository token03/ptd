#include "raylib.h"
#include "rlImGui.h"

int main() {
  const int screenWidth = 1280;
  const int screenHeight = 720;
  rlImGuiSetup(true);
  InitWindow(screenWidth, screenHeight, "ptd");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(RAYWHITE);

    DrawText("ptd", 20, 20, 20, DARKGRAY);

    rlImGuiEnd();
    EndDrawing();
  }

  CloseWindow();
  rlImGuiShutdown();

  return 0;
}
