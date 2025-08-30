#pragma once

#include <string>
#include <vector>

enum class PokemonType {
  None,
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
  Dark,
  Steel,
  Fairy
};

enum class Gender { Male, Female, Genderless };

enum class Nature {
  Hardy,
  Lonely,
  Brave,
  Adamant,
  Naughty,
  Bold,
  Docile,
  Relaxed,
  Impish,
  Lax,
  Timid,
  Hasty,
  Serious,
  Jolly,
  Naive,
  Modest,
  Mild,
  Quiet,
  Bashful,
  Rash,
  Calm,
  Gentle,
  Sassy,
  Careful,
  Quirky
};

enum Stat { Attack, Defense, SpAttack, SpDefense, Speed, HP };

struct BaseStats {
  int hp = 0, attack = 0, defense = 0, spAttack = 0, spDefense = 0, speed = 0;
};

struct IVs {
  int hp = 0, attack = 0, defense = 0, spAttack = 0, spDefense = 0, speed = 0;
};

struct EVs {
  int hp = 0, attack = 0, defense = 0, spAttack = 0, spDefense = 0, speed = 0;
};

struct SpeciesData {
  std::string dexNumber;
  std::string speciesName;
  std::vector<PokemonType> types;
  BaseStats baseStats;
};

struct PokemonInstanceConfig {
  int level = 1;
  Nature nature = Nature::Hardy;
  IVs ivs;
  EVs evs;
  Gender gender = Gender::Genderless;
  bool isShiny = false;
};
