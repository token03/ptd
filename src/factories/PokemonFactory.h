#pragma once

#include "core/GameObject.h"
#include "raylib.h"
#include <memory>
#include <string>

class PMDLoader;

class PokemonFactory {
public:
  PokemonFactory(std::shared_ptr<PMDLoader> loader);

  std::shared_ptr<GameObject>
  CreatePokemonObject(const std::string &formId,
                      const std::string &initialAnimation,
                      Vector2 position = {0, 0}, Vector2 scale = {1, 1});

private:
  std::shared_ptr<PMDLoader> m_loader;
};
