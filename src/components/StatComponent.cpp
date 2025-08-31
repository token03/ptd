#include "components/StatComponent.h"

#include <cmath>
#include <map>
#include <memory>

#include "core/GameObject.h"  // IWYU pragma: keep - required for assignRequiredComponent
#include "spdlog/spdlog.h"

static std::map<Nature, std::pair<Stat, Stat>> natureStatChanges = {
    {Nature::HARDY, {Stat::ATTACK, Stat::ATTACK}},
    {Nature::DOCILE, {Stat::DEFENSE, Stat::DEFENSE}},
    {Nature::SERIOUS, {Stat::SPEED, Stat::SPEED}},
    {Nature::BASHFUL, {Stat::SPATTACK, Stat::SPATTACK}},
    {Nature::QUIRKY, {Stat::SPDEFENSE, Stat::SPDEFENSE}},

    {Nature::LONELY, {Stat::ATTACK, Stat::DEFENSE}},
    {Nature::BRAVE, {Stat::ATTACK, Stat::SPEED}},
    {Nature::ADAMANT, {Stat::ATTACK, Stat::SPATTACK}},
    {Nature::NAUGHTY, {Stat::ATTACK, Stat::SPDEFENSE}},

    {Nature::BOLD, {Stat::DEFENSE, Stat::ATTACK}},
    {Nature::RELAXED, {Stat::DEFENSE, Stat::SPEED}},
    {Nature::IMPISH, {Stat::DEFENSE, Stat::SPATTACK}},
    {Nature::LAX, {Stat::DEFENSE, Stat::SPDEFENSE}},

    {Nature::TIMID, {Stat::SPEED, Stat::ATTACK}},
    {Nature::HASTY, {Stat::SPEED, Stat::DEFENSE}},
    {Nature::JOLLY, {Stat::SPEED, Stat::SPATTACK}},
    {Nature::NAIVE, {Stat::SPEED, Stat::SPDEFENSE}},

    {Nature::MODEST, {Stat::SPATTACK, Stat::ATTACK}},
    {Nature::MILD, {Stat::SPATTACK, Stat::DEFENSE}},
    {Nature::QUIET, {Stat::SPATTACK, Stat::SPEED}},
    {Nature::RASH, {Stat::SPATTACK, Stat::SPDEFENSE}},

    {Nature::CALM, {Stat::SPDEFENSE, Stat::ATTACK}},
    {Nature::GENTLE, {Stat::SPDEFENSE, Stat::DEFENSE}},
    {Nature::SASSY, {Stat::SPDEFENSE, Stat::SPEED}},
    {Nature::CAREFUL, {Stat::SPDEFENSE, Stat::SPATTACK}},
};

float getNatureModifier(Nature nature, Stat stat) {
  auto it = natureStatChanges.find(nature);
  if (it != natureStatChanges.end()) {
    if (it->second.first == stat) return 1.1f;
    if (it->second.second == stat) return 0.9f;
  }
  return 1.0f;
}

StatComponent::StatComponent(int initialLevel, Stats initialIVs, Stats initialEVs)
    : level(initialLevel),
      m_ivs(initialIVs),
      m_evs(initialEVs),
      maxHp(0),
      currentHp(0),
      attack(0),
      defense(0),
      spAttack(0),
      spDefense(0),
      speed(0) {}

void StatComponent::Init() {
  assignRequiredComponent(m_species, m_traits);
  CalculateStats();
}

void StatComponent::CalculateStats() {
  auto species = m_species.lock();
  auto traits = m_traits.lock();

  if (!species || !traits) {
    spdlog::error("Cannot calculate stats, required components are missing!");
    return;
  }

  const Stats &base = species->baseStats;

  maxHp = (((2 * base.hp + m_ivs.hp + (m_evs.hp / 4)) * level) / 100) + level + 10;

  auto calculate_stat = [&](int base_val, int iv, int ev, Stat stat_enum) {
    float natureMod = getNatureModifier(traits->nature, stat_enum);
    int stat = (((2 * base_val + iv + (ev / 4)) * level) / 100) + 5;
    return static_cast<int>(std::floor(stat * natureMod));
  };

  attack = calculate_stat(base.attack, m_ivs.attack, m_evs.attack, Stat::ATTACK);
  defense = calculate_stat(base.defense, m_ivs.defense, m_evs.defense, Stat::DEFENSE);
  spAttack =
      calculate_stat(base.spAttack, m_ivs.spAttack, m_evs.spAttack, Stat::SPATTACK);
  spDefense =
      calculate_stat(base.spDefense, m_ivs.spDefense, m_evs.spDefense, Stat::SPDEFENSE);
  speed = calculate_stat(base.speed, m_ivs.speed, m_evs.speed, Stat::SPEED);

  currentHp = maxHp;

  spdlog::info("{} at level {} has {} HP, {} ATK.", species->speciesName, level, maxHp,
               attack);
}

void StatComponent::SetLevel(int newLevel) {
  level = newLevel;
  CalculateStats();
}

void StatComponent::TakeDamage(int damage) {
  int damageTaken = damage - (defense / 2);
  if (damageTaken < 1) damageTaken = 1;

  currentHp -= damageTaken;
  if (currentHp < 0) {
    currentHp = 0;
  }
}

bool StatComponent::IsFainted() const { return currentHp <= 0; }
