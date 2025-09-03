#pragma once

#include "core/Component.h"
#include "data/PokedexData.h"

class PokedexComponent : public Component {
 public:
  explicit PokedexComponent(const PokedexData &data);

  const PokedexData data;
};
