#include "components/SpeciesComponent.h"

SpeciesComponent::SpeciesComponent(std::string dexNum, std::string name,
                                   std::vector<PokemonType> pkmTypes,
                                   BaseStats stats)
    : dexNumber(std::move(dexNum)), speciesName(std::move(name)),
      types(std::move(pkmTypes)), baseStats(stats) {}
