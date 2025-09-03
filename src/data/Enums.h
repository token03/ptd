#pragma once

#include <glaze/glaze.hpp>

enum class PokemonType {
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
  FAIRY,
  BIRD,
};

template <>
struct glz::meta<PokemonType> {
  using enum PokemonType;
  static constexpr auto value = enumerate(
      // clang-format off
      "Normal", NORMAL, 
      "Fire", FIRE, 
      "Water", WATER, 
      "Grass", GRASS, 
      "Electric", ELECTRIC, 
      "Ice", ICE, 
      "Fighting", FIGHTING, 
      "Poison", POISON, 
      "Ground", GROUND,
      "Flying", FLYING, 
      "Psychic", PSYCHIC, 
      "Bug", BUG, 
      "Rock", ROCK, 
      "Ghost", GHOST,
      "Dragon", DRAGON, 
      "Dark", DARK, 
      "Steel", STEEL, 
      "Fairy", FAIRY,
      "Bird", BIRD
      // clang-format on
  );
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
