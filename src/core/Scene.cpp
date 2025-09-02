#include "core/Scene.h"

#include <algorithm>

#include "components/gameplay/TagComponent.h"
#include "core/GameObject.h"
#include "managers/SceneManager.h"

Scene::~Scene() = default;

void Scene::Update(float deltaTime) {
  if (!m_spawnQueue.empty()) {
    for (auto& go : m_spawnQueue) {
      AddGameObject(go);
    }
    m_spawnQueue.clear();
  }

  for (auto& go : m_gameObjects) {
    go->Update(deltaTime);
  }

  m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
                                     [](const std::shared_ptr<GameObject>& go) {
                                       return go->IsDestroyed();
                                     }),
                      m_gameObjects.end());
}

void Scene::Draw(float deltaTime) {
  for (auto& go : m_gameObjects) {
    go->Draw();
  }
}

bool Scene::IsOverlay() const { return false; }

std::shared_ptr<GameObject> Scene::AddGameObject(std::shared_ptr<GameObject> go) {
  go->SetScene(weak_from_this());
  m_gameObjects.push_back(go);
  return go;
}

std::shared_ptr<GameObject> Scene::CreateGameObject() {
  auto go = std::make_shared<GameObject>();
  return AddGameObject(go);
}

void Scene::EnqueueGameObject(std::shared_ptr<GameObject> go) {
  m_spawnQueue.push_back(go);
}

const std::vector<std::shared_ptr<GameObject>>& Scene::GetGameObjects() const {
  return m_gameObjects;
}

std::vector<std::shared_ptr<GameObject>> Scene::GetGameObjectsWithTag(
    const std::string& tag) {
  std::vector<std::shared_ptr<GameObject>> taggedObjects;
  for (const auto& go : m_gameObjects) {
    if (auto tagComp = go->GetComponentShared<TagComponent>()) {  // One lookup
      if (tagComp->tag == tag) {
        taggedObjects.push_back(go);
      }
    }
  }
  return taggedObjects;
}

void Scene::SetSceneManager(std::weak_ptr<SceneManager> manager) {
  m_sceneManager = manager;
}
