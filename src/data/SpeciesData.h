#pragma once

#include "Enums.h"
#include "Stats.h"
#include <glaze/glaze.hpp>
#include <string>
#include <vector>

struct SpeciesData {
  std::string dexNumber;
  std::string speciesName;
  std::vector<PokemonType> types;
  BaseStats baseStats;
};

template <> struct glz::meta<SpeciesData> {
  using T = SpeciesData;
  static constexpr auto value =
      glz::object("dexNumber", &T::dexNumber, "speciesName", &T::speciesName,
                  "types", &T::types, "baseStats", &T::baseStats);
};
