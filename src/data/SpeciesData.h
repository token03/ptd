#pragma once

#include <glaze/glaze.hpp>
#include <string>
#include <vector>

#include "Enums.h"
#include "Stats.h"

struct SpeciesData {
  std::string dexNumber;
  std::string speciesName;
  std::vector<PokemonType> types;
  Stats baseStats;
};

template <>
struct glz::meta<SpeciesData> {
  using T = SpeciesData;
  static constexpr auto value =
      glz::object("dexNumber", &T::dexNumber, "speciesName", &T::speciesName, "types",
                  &T::types, "baseStats", &T::baseStats);
};
