#pragma once

#include <memory>

#include "components/SpeciesComponent.h"
#include "components/TraitsComponent.h"
#include "core/Component.h"
#include "data/Stats.h"

class StatComponent : public Component {
 public:
  StatComponent(int initialLevel, Stats initialIVs, Stats initialEVs);
  void Init() override;

  void CalculateStats();
  void SetLevel(int newLevel);
  void TakeDamage(int damage);
  bool IsFainted() const;

  int level;
  int maxHp, currentHp;
  int attack, defense, spAttack, spDefense, speed;

 private:
  Stats m_ivs;
  Stats m_evs;

  std::weak_ptr<SpeciesComponent> m_species;
  std::weak_ptr<TraitsComponent> m_traits;
};
