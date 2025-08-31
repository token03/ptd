#include "components/StatComponent.h"
#include "core/GameObject.h" // IWYU pragma: keep - required for assignRequiredComponent
#include "spdlog/spdlog.h"
#include <cmath>
#include <map>
#include <memory>

static std::map<Nature, std::pair<Stat, Stat>> natureStatChanges = {
    {Nature::Hardy, {Stat::Attack, Stat::Attack}},
    {Nature::Docile, {Stat::Defense, Stat::Defense}},
    {Nature::Serious, {Stat::Speed, Stat::Speed}},
    {Nature::Bashful, {Stat::SpAttack, Stat::SpAttack}},
    {Nature::Quirky, {Stat::SpDefense, Stat::SpDefense}},

    {Nature::Lonely, {Stat::Attack, Stat::Defense}},
    {Nature::Brave, {Stat::Attack, Stat::Speed}},
    {Nature::Adamant, {Stat::Attack, Stat::SpAttack}},
    {Nature::Naughty, {Stat::Attack, Stat::SpDefense}},

    {Nature::Bold, {Stat::Defense, Stat::Attack}},
    {Nature::Relaxed, {Stat::Defense, Stat::Speed}},
    {Nature::Impish, {Stat::Defense, Stat::SpAttack}},
    {Nature::Lax, {Stat::Defense, Stat::SpDefense}},

    {Nature::Timid, {Stat::Speed, Stat::Attack}},
    {Nature::Hasty, {Stat::Speed, Stat::Defense}},
    {Nature::Jolly, {Stat::Speed, Stat::SpAttack}},
    {Nature::Naive, {Stat::Speed, Stat::SpDefense}},

    {Nature::Modest, {Stat::SpAttack, Stat::Attack}},
    {Nature::Mild, {Stat::SpAttack, Stat::Defense}},
    {Nature::Quiet, {Stat::SpAttack, Stat::Speed}},
    {Nature::Rash, {Stat::SpAttack, Stat::SpDefense}},

    {Nature::Calm, {Stat::SpDefense, Stat::Attack}},
    {Nature::Gentle, {Stat::SpDefense, Stat::Defense}},
    {Nature::Sassy, {Stat::SpDefense, Stat::Speed}},
    {Nature::Careful, {Stat::SpDefense, Stat::SpAttack}},
};

float getNatureModifier(Nature nature, Stat stat) {
  auto it = natureStatChanges.find(nature);
  if (it != natureStatChanges.end()) {
    if (it->second.first == stat)
      return 1.1f;
    if (it->second.second == stat)
      return 0.9f;
  }
  return 1.0f;
}

StatComponent::StatComponent(int initialLevel, Stats initialIVs,
                             Stats initialEVs)
    : level(initialLevel), m_ivs(initialIVs), m_evs(initialEVs), maxHp(0),
      currentHp(0), attack(0), defense(0), spAttack(0), spDefense(0), speed(0) {
}

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

  maxHp =
      (((2 * base.hp + m_ivs.hp + (m_evs.hp / 4)) * level) / 100) + level + 10;

  auto calculate_stat = [&](int base_val, int iv, int ev, Stat stat_enum) {
    float natureMod = getNatureModifier(traits->nature, stat_enum);
    int stat = (((2 * base_val + iv + (ev / 4)) * level) / 100) + 5;
    return static_cast<int>(std::floor(stat * natureMod));
  };

  attack =
      calculate_stat(base.attack, m_ivs.attack, m_evs.attack, Stat::Attack);
  defense =
      calculate_stat(base.defense, m_ivs.defense, m_evs.defense, Stat::Defense);
  spAttack = calculate_stat(base.spAttack, m_ivs.spAttack, m_evs.spAttack,
                            Stat::SpAttack);
  spDefense = calculate_stat(base.spDefense, m_ivs.spDefense, m_evs.spDefense,
                             Stat::SpDefense);
  speed = calculate_stat(base.speed, m_ivs.speed, m_evs.speed, Stat::Speed);

  currentHp = maxHp;

  spdlog::info("{} at level {} has {} HP, {} ATK.", species->speciesName, level,
               maxHp, attack);
}

void StatComponent::SetLevel(int newLevel) {
  level = newLevel;
  CalculateStats();
}

void StatComponent::TakeDamage(int damage) {
  int damageTaken = damage - (defense / 2);
  if (damageTaken < 1)
    damageTaken = 1;

  currentHp -= damageTaken;
  if (currentHp < 0) {
    currentHp = 0;
  }
}

bool StatComponent::IsFainted() const { return currentHp <= 0; }
