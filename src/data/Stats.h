#pragma once

#include <glaze/glaze.hpp>

#include "Enums.h"

struct BaseStats {
  int hp = 0, attack = 0, defense = 0, spAttack = 0, spDefense = 0, speed = 0;
};

struct IVs {
  int hp = 0, attack = 0, defense = 0, spAttack = 0, spDefense = 0, speed = 0;
};
struct EVs {
  int hp = 0, attack = 0, defense = 0, spAttack = 0, spDefense = 0, speed = 0;
};

template <> struct glz::meta<BaseStats> {
  using T = BaseStats;
  static constexpr auto value = glz::object(
      "hp", &T::hp, "attack", &T::attack, "defense", &T::defense, "spAttack",
      &T::spAttack, "spDefense", &T::spDefense, "speed", &T::speed);
};

template <> struct glz::meta<IVs> {
  using T = IVs;
  static constexpr auto value = glz::object(
      "hp", &T::hp, "attack", &T::attack, "defense", &T::defense, "spAttack",
      &T::spAttack, "spDefense", &T::spDefense, "speed", &T::speed);
};
template <> struct glz::meta<EVs> {
  using T = EVs;
  static constexpr auto value = glz::object(
      "hp", &T::hp, "attack", &T::attack, "defense", &T::defense, "spAttack",
      &T::spAttack, "spDefense", &T::spDefense, "speed", &T::speed);
};
