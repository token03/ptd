#pragma once

#include <glaze/glaze.hpp>

enum class PokemonType {
  NONE,
  NORMAL,
  FIRE,
  WATER,
  GRASS,
  ELECTRIC,
  ICE,
  FIGHTING,
  POISON,
  GROUND,
  FLYING,
  PSYCHIC,
  BUG,
  ROCK,
  GHOST,
  DRAGON,
  DARK,
  STEEL,
  FAIRY
};

template <> struct glz::meta<PokemonType> {
  using enum PokemonType;
  static constexpr auto value = enumerate(
      NONE, NORMAL, FIRE, WATER, GRASS, ELECTRIC, ICE, FIGHTING, POISON, GROUND,
      FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON, DARK, STEEL, FAIRY);
};

enum class Gender { Male, Female, Genderless };

enum class Nature {
  HARDY,
  LONELY,
  BRAVE,
  ADAMANT,
  NAUGHTY,
  BOLD,
  DOCILE,
  RELAXED,
  IMPISH,
  LAX,
  TIMID,
  HASTY,
  SERIOUS,
  JOLLY,
  NAIVE,
  MODEST,
  MILD,
  QUIET,
  BASHFUL,
  RASH,
  CALM,
  GENTLE,
  SASSY,
  CAREFUL,
  QUIRKY
};

enum Stat { ATTACK, DEFENSE, SPATTACK, SPDEFENSE, SPEED, HP };
