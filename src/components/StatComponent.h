#pragma once

#include "core/Component.h"
#include "data/Stats.h"

class SpeciesComponent;
class PersonalityComponent;

class StatComponent : public Component {
public:
  StatComponent(int initialLevel, IVs initialIVs, EVs initialEVs);
  void Init() override;

  void CalculateStats();
  void SetLevel(int newLevel);
  void TakeDamage(int damage);
  bool IsFainted() const;

  int level;
  int maxHp, currentHp;
  int attack, defense, spAttack, spDefense, speed;

private:
  IVs m_ivs;
  EVs m_evs;

  SpeciesComponent *m_species = nullptr;
  PersonalityComponent *m_personality = nullptr;
};
