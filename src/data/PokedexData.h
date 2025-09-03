#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "data/Enums.h"
#include "data/Stats.h"

enum class SpeciesTag { MYTHICAL, LEGENDARY, SUB_LEGENDARY, ULTRA_BEAST, PARADOX };

template <>
struct glz::meta<SpeciesTag> {
  using enum SpeciesTag;
  static constexpr auto value = glz::enumerate(
      "Mythical", MYTHICAL, "Restricted Legendary", LEGENDARY, "Sub-Legendary",
      SUB_LEGENDARY, "Ultra Beast", ULTRA_BEAST, "Paradox", PARADOX);
};

enum class EvoType {
  TRADE,
  USE_ITEM,
  LEVEL_MOVE,
  LEVEL_EXTRA,
  LEVEL_FRIENDSHIP,
  LEVEL_HOLD,
  OTHER
};

template <>
struct glz::meta<EvoType> {
  using enum EvoType;
  static constexpr auto value =
      glz::enumerate("trade", TRADE, "useItem", USE_ITEM, "levelMove", LEVEL_MOVE,
                     "levelExtra", LEVEL_EXTRA, "levelFriendship", LEVEL_FRIENDSHIP,
                     "levelHold", LEVEL_HOLD, "other", OTHER);
};

enum class EvoRegion { ALOLA, GALAR };

template <>
struct glz::meta<EvoRegion> {
  using enum EvoRegion;
  static constexpr auto value = glz::enumerate("Alola", ALOLA, "Galar", GALAR);
};

struct GenderRatio {
  double M = 0;
  double F = 0;
};

template <>
struct glz::meta<GenderRatio> {
  using T = GenderRatio;
  static constexpr auto value = glz::object("M", &T::M, "F", &T::F);
};

struct Abilities {
  std::string zero;
  std::optional<std::string> one;
  std::optional<std::string> H;
  std::optional<std::string> S;
};

template <>
struct glz::meta<Abilities> {
  using T = Abilities;
  static constexpr auto value =
      glz::object("0", &T::zero, "1", &T::one, "H", &T::H, "S", &T::S);
};

struct PokedexData {
  // Core Data
  int num;
  std::string name;
  std::vector<PokemonType> types;
  std::optional<std::string> addedType;
  Stats baseStats;
  Abilities abilities;
  double heightm;
  double weightkg;
  std::string color;
  std::optional<int> gen;
  std::optional<std::vector<SpeciesTag>> tags;
  std::optional<int> maxHP;

  // Evolution Data
  std::optional<std::string> prevo;
  std::optional<std::vector<std::string>> evos;
  std::optional<EvoType> evoType;
  std::optional<std::string> evoCondition;
  std::optional<std::string> evoItem;
  std::optional<std::string> evoMove;
  std::optional<EvoRegion> evoRegion;
  std::optional<int> evoLevel;

  // Breeding Data
  std::vector<std::string> eggGroups;
  std::optional<bool> canHatch;
  std::optional<std::string> gender;
  std::optional<GenderRatio> genderRatio;
  std::optional<std::string> mother;

  // Forme Data
  std::optional<std::string> baseSpecies;
  std::optional<std::string> baseForme;
  std::optional<std::string> forme;
  std::optional<std::vector<std::string>> cosmeticFormes;
  std::optional<std::vector<std::string>> otherFormes;
  std::optional<std::vector<std::string>> formeOrder;
  std::optional<std::string> changesFrom;
  std::optional<std::variant<std::string, std::vector<std::string>>> battleOnly;

  // Gimmick Data / Special Abilities
  std::optional<std::variant<bool, std::string>> unreleasedHidden;
  std::optional<bool> maleOnlyHidden;
  std::optional<std::string> canGigantamax;
  std::optional<bool> gmaxUnreleased;
  std::optional<bool> cannotDynamax;
  std::optional<std::string> requiredTeraType;
  std::optional<std::string> requiredItem;
  std::optional<std::vector<std::string>> requiredItems;
  std::optional<std::string> requiredMove;
  std::optional<std::string> requiredAbility;

  // Format Data
  std::optional<std::string> tier;
  std::optional<std::string> doublesTier;
  std::optional<std::string> natDexTier;
  std::optional<std::string> isNonstandard;
};

template <>
struct glz::meta<PokedexData> {
  using T = PokedexData;
  static constexpr auto value = glz::object(
      // Core Data
      "num", &T::num, "name", &T::name, "types", &T::types, "addedType", &T::addedType,
      "baseStats", &T::baseStats, "abilities", &T::abilities, "heightm", &T::heightm,
      "weightkg", &T::weightkg, "color", &T::color, "gen", &T::gen, "tags", &T::tags,
      "maxHP", &T::maxHP,

      // Evolution Data
      "prevo", &T::prevo, "evos", &T::evos, "evoType", &T::evoType, "evoCondition",
      &T::evoCondition, "evoItem", &T::evoItem, "evoMove", &T::evoMove, "evoRegion",
      &T::evoRegion, "evoLevel", &T::evoLevel,

      // Breeding Data
      "eggGroups", &T::eggGroups, "canHatch", &T::canHatch, "gender", &T::gender,
      "genderRatio", &T::genderRatio, "mother", &T::mother,

      // Forme Data
      "baseSpecies", &T::baseSpecies, "baseForme", &T::baseForme, "forme", &T::forme,
      "cosmeticFormes", &T::cosmeticFormes, "otherFormes", &T::otherFormes, "formeOrder",
      &T::formeOrder, "changesFrom", &T::changesFrom, "battleOnly", &T::battleOnly,

      // Gimmick Data / Special Abilities
      "unreleasedHidden", &T::unreleasedHidden, "maleOnlyHidden", &T::maleOnlyHidden,
      "canGigantamax", &T::canGigantamax, "gmaxUnreleased", &T::gmaxUnreleased,
      "cannotDynamax", &T::cannotDynamax, "requiredTeraType", &T::requiredTeraType,
      "requiredItem", &T::requiredItem, "requiredItems", &T::requiredItems,
      "requiredMove", &T::requiredMove, "requiredAbility", &T::requiredAbility,

      // Format Data
      "tier", &T::tier, "doublesTier", &T::doublesTier, "natDexTier", &T::natDexTier,
      "isNonstandard", &T::isNonstandard);
};
