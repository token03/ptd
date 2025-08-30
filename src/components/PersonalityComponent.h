#pragma once

#include "core/Component.h"
#include "data/PokemonData.h"

class PersonalityComponent : public Component {
public:
  PersonalityComponent(Nature p_nature, Gender p_gender, bool p_isShiny);

  Nature nature;
  Gender gender;
  bool isShiny;
};
