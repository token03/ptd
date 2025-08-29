#pragma once

#include "core/Component.h"

class SpeciesComponent;

class StatComponent : public Component {
public:
  explicit StatComponent(int initialLevel);

  void Init() override;

  void TakeDamage(int damage);
  bool IsFainted() const;
  void SetLevel(int newLevel);

  int level;
  int maxHp;
  int currentHp;
  int attack;
  int spAttack;
  int defense;
  int spDefense;
  int speed;

private:
  void CalculateStats();

  const SpeciesComponent *m_species = nullptr;
};
