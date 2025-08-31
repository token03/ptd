#include "components/SpeciesComponent.h"

SpeciesComponent::SpeciesComponent(const SpeciesData &data)
    : dexNumber(data.dexNumber),
      speciesName(data.speciesName),
      types(data.types),
      baseStats(data.baseStats) {}
