#pragma once

#include <glaze/glaze.hpp>
#include <string>

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

template <> struct glz::meta<PokemonType> {
  using enum PokemonType;
  static constexpr auto value = enumerate(
      None, Normal, Fire, Water, Grass, Electric, Ice, Fighting, Poison, Ground,
      Flying, Psychic, Bug, Rock, Ghost, Dragon, Dark, Steel, Fairy);
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
