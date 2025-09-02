#pragma once

#include "core/Scene.h"
#include "raylib.h"

class TextureManager;
class DataManager;
class MobFactory;
class TowerFactory;

class GameplayScene : public Scene {
 public:
  GameplayScene(std::shared_ptr<TextureManager> assetManager,
                std::shared_ptr<DataManager> dataManager);
  ~GameplayScene() override;

  void Load() override;
  void Unload() override;
  void Update(float deltaTime) override;
  void Draw(float deltaTime) override;

 private:
  void LoadTestData();
  void DrawBackground() const;

  const int screenWidth = 1280;
  const int screenHeight = 720;

  std::shared_ptr<TextureManager> m_assetManager;
  std::shared_ptr<DataManager> m_dataManager;

  std::shared_ptr<TowerFactory> m_towerFactory;
  std::shared_ptr<MobFactory> m_mobFactory;

  // NEW: Background texture member
  Texture2D m_backgroundTexture;
  bool m_backgroundLoaded = false;
};
