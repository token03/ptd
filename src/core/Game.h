#pragma once
#include "core/GameObject.h"
#include "raylib.h"
#include "utils/PMDLoader.h"
#include <memory>
#include <vector>

class Game {
public:
  Game();
  ~Game(); // Add a destructor to clean up resources
  int Run();

private:
  void Load(); // Helper to load assets and create objects
  void Update(float deltaTime);
  void Draw();

private:
  const int screenWidth = 1280;
  const int screenHeight = 720;

  std::unique_ptr<PMDLoader> m_loader;
  // GameObjects are now managed by shared_ptr for safe, shared ownership
  std::vector<std::shared_ptr<GameObject>> m_gameObjects;

  // Example texture, a real game would have a resource manager
  Texture2D m_slowkingTexture;
};
