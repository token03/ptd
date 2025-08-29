#pragma once

#include "core/Component.h"
#include <string>
#include <vector>

enum class PokemonType {
  Normal,
  Fire,
  Water,
  Grass,
  Electric,
  Ice,
  Fighting,
  Poison,
  Ground,
  Flying,
  Psychic,
  Bug,
  Rock,
  Ghost,
  Dragon,
  Steel,
  Dark,
  Fairy
};

struct BaseStats {
  int hp;
  int attack;
  int defense;
  int specialAttack;
  int specialDefense;
  int speed;
};

class SpeciesComponent : public Component {
public:
  SpeciesComponent(std::string dexNum, std::string name,
                   std::vector<PokemonType> pkmTypes, BaseStats stats);

  const std::string dexNumber;
  const std::string speciesName;
  const std::vector<PokemonType> types;
  const BaseStats baseStats;
};
