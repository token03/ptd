#pragma once
#include <glaze/glaze.hpp>

struct Stats {
  int hp = 0;
  int attack = 0;
  int defense = 0;
  int spAttack = 0;
  int spDefense = 0;
  int speed = 0;
};

template <> struct glz::meta<Stats> {
  using T = Stats;
  static constexpr auto value = glz::object(
      "hp", &T::hp, "attack", &T::attack, "defense", &T::defense, "spAttack",
      &T::spAttack, "spDefense", &T::spDefense, "speed", &T::speed);
};
