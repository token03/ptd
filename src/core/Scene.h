#pragma once

#include <memory>
#include <string>
#include <vector>

class GameObject;
class SceneManager;

class Scene : public std::enable_shared_from_this<Scene> {
 public:
  virtual ~Scene();

  virtual void Load() = 0;
  virtual void Unload() = 0;

  virtual void Update(float deltaTime);
  virtual void Draw(float deltaTime);

  virtual bool IsOverlay() const;

  std::shared_ptr<GameObject> AddGameObject(std::shared_ptr<GameObject> go);
  std::shared_ptr<GameObject> CreateGameObject();
  void EnqueueGameObject(std::shared_ptr<GameObject> go);

  const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const;
  std::vector<std::shared_ptr<GameObject>> GetGameObjectsWithTag(const std::string& tag);

  void SetSceneManager(std::weak_ptr<SceneManager> manager);

 protected:
  std::vector<std::shared_ptr<GameObject>> m_gameObjects;
  std::vector<std::shared_ptr<GameObject>> m_spawnQueue;

  std::weak_ptr<SceneManager> m_sceneManager;
};
