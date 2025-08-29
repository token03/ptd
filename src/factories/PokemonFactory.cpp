#include "PokemonFactory.h"
#include "components/AnimationComponent.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"
#include "spdlog/spdlog.h"
#include "utils/PMDLoader.h"

PokemonFactory::PokemonFactory(std::shared_ptr<PMDLoader> loader)
    : m_loader(std::move(loader)) {}

std::shared_ptr<GameObject>
PokemonFactory::CreatePokemonObject(const std::string &formId,
                                    const std::string &initialAnimation,
                                    Vector2 position, Vector2 scale) {
  if (!m_loader) {
    spdlog::error("PokemonFactory error: PMDLoader is not available.");
    return nullptr;
  }

  const PMDData *form = m_loader->getForm(formId);
  if (!form || !form->animData) {
    spdlog::error("Could not create GameObject for form ID: {}. Form data not "
                  "loaded or has no animations.",
                  formId);
    return nullptr;
  }

  Texture2D texture = m_loader->getAnimationTexture(formId, initialAnimation);
  if (texture.id <= 0) {
    spdlog::error("Could not load initial texture for animation: {}",
                  initialAnimation);
    return nullptr;
  }

  auto gameObject = std::make_shared<GameObject>();

  auto &transform = gameObject->AddComponent<TransformComponent>();
  transform.position = position;
  transform.scale = scale;

  gameObject->AddComponent<SpriteComponent>(texture);
  gameObject->AddComponent<AnimationComponent>(m_loader, formId);

  gameObject->GetComponent<AnimationComponent>().Play(initialAnimation);

  return gameObject;
}
