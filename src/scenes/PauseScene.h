#pragma once

#include "core/Scene.h"

class PauseScene : public Scene {
 public:
  PauseScene();
  ~PauseScene() override;

  void Load() override;
  void Unload() override;

  void Update(float deltaTime) override;
  void Draw(float deltaTime) override;

  bool IsOverlay() const override;
};
