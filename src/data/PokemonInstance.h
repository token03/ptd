#pragma once

#include "Enums.h"
#include "Stats.h"

struct PokemonInstance {
  int level = 1;
  Nature nature = Nature::Hardy;
  IVs ivs;
  EVs evs;
  Gender gender = Gender::Genderless;
  bool isShiny = false;
};
