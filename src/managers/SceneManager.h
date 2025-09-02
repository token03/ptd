#pragma once

#include <memory>
#include <vector>

class Scene;

class SceneManager : public std::enable_shared_from_this<SceneManager> {
 public:
  SceneManager();
  ~SceneManager();

  void Update(float deltaTime);
  void Draw(float deltaTime);

  void PushScene(std::shared_ptr<Scene> scene);
  void PopScene();
  void ChangeScene(std::shared_ptr<Scene> scene);

  std::shared_ptr<Scene> GetCurrentScene() const;
  bool IsEmpty() const;

 private:
  std::vector<std::shared_ptr<Scene>> m_scenes;
};
