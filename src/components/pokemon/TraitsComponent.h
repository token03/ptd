#pragma once

#include "core/Component.h"
#include "data/Enums.h"

class TraitsComponent : public Component {
 public:
  TraitsComponent(Nature p_nature, Gender p_gender, bool p_isShiny);
  int level;
  Nature nature;
  Gender gender;
  bool isShiny;
  int friendShip;
};
