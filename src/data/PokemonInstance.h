#pragma once

#include "Enums.h"
#include "Stats.h"

struct PokemonInstance {
  int level = 1;
  Nature nature = Nature::HARDY;
  Stats ivs;
  Stats evs;
  Gender gender = Gender::Genderless;
  bool isShiny = false;
};
