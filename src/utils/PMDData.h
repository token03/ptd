#pragma once

#include <glaze/glaze.hpp>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

struct Animation {
  std::string name;
  int index;
  int frameWidth;
  int frameHeight;
  std::optional<int> rushFrame;
  std::optional<int> hitFrame;
  std::optional<int> returnFrame;
  std::vector<int> durations;
  std::optional<std::string> copyOf;
};

struct AnimationData {
  int shadowSize;
  std::map<std::string, Animation> animations;
};

struct Credit {
  std::string primary = "Unknown";
  std::vector<std::string> secondary;
};

template <> struct glz::meta<Credit> {
  using T = Credit;
  static constexpr auto value =
      object("primary", &T::primary, "secondary", &T::secondary);
};

struct TrackerEntry;
using SubgroupsMap = std::map<std::string, TrackerEntry>;

struct TrackerEntry {
  std::string name;
  Credit sprite_credit;
  Credit portrait_credit;
  std::map<std::string, bool> portrait_files;
  SubgroupsMap subgroups;
};

template <> struct glz::meta<TrackerEntry> {
  using T = TrackerEntry;
  static constexpr auto value =
      object("name", &T::name, "sprite_credit", &T::sprite_credit,
             "portrait_credit", &T::portrait_credit, "portrait_files",
             &T::portrait_files, "subgroups", &T::subgroups);
};

class PokemonForm {
public:
  std::string fullId;
  std::string fullName;
  std::string dex;
  std::string formPath;

  Credit spriteCredit;
  std::optional<AnimationData> animData;
  std::vector<std::string> animFileBases;

  Credit portraitCredit;
  std::set<std::string> availablePortraits;
};
