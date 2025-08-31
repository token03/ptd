#pragma once

#include "core/Component.h"
#include "data/Stats.h"
#include <memory>

class SpeciesComponent;
class PersonalityComponent;

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
  std::weak_ptr<PersonalityComponent> m_personality;
};
