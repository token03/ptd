#pragma once

#include "Enums.h"
#include <glaze/glaze.hpp>
#include <vector>

struct EffectivenessData {
  PokemonType against;
  double value;
};

struct TypeData {
  PokemonType type;
  std::vector<EffectivenessData> effectiveness;
};

template <> struct glz::meta<EffectivenessData> {
  using T = EffectivenessData;
  static constexpr auto value =
      glz::object("against", &T::against, "value", &T::value);
};

template <> struct glz::meta<TypeData> {
  using T = TypeData;
  static constexpr auto value =
      glz::object("type", &T::type, "effectiveness", &T::effectiveness);
};
