#include "PokemonFactory.h"
#include "components/AnimationComponent.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"
#include "utils/PMDLoader.h"
#include <iostream>

PokemonFactory::PokemonFactory(std::shared_ptr<PMDLoader> loader)
    : m_loader(std::move(loader)) {}

std::shared_ptr<GameObject>
PokemonFactory::CreatePokemonObject(const std::string &formId,
                                    const std::string &initialAnimation,
                                    Vector2 position, Vector2 scale) {
  if (!m_loader) {
    std::cerr << "PokemonFactory error: PMDLoader is not available."
              << std::endl;
    return nullptr;
  }

  const PokemonForm *form = m_loader->getForm(formId);
  if (!form || !form->animData) {
    std::cerr << "Could not create GameObject for form ID: " << formId
              << ". Form data not loaded or has no animations." << std::endl;
    return nullptr;
  }

  Texture2D texture = m_loader->getAnimationTexture(formId, initialAnimation);
  if (texture.id <= 0) {
    std::cerr << "Could not load initial texture for animation: "
              << initialAnimation << std::endl;
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
