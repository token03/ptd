#pragma once

#include "core/Component.h"
#include "data/Enums.h"
#include "data/SpeciesData.h"
#include "data/Stats.h"
#include <string>
#include <vector>

class SpeciesComponent : public Component {
public:
  explicit SpeciesComponent(const SpeciesData &data);

  std::string dexNumber;
  std::string speciesName;
  std::vector<PokemonType> types;
  Stats baseStats;
};
