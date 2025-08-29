#include "components/SpeciesComponent.h"
#include "components/StatComponent.h"
#include "core/GameObject.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <stdexcept>

StatComponent::StatComponent(int initialLevel)
    : level(initialLevel), maxHp(0), currentHp(0), attack(0), defense(0) {}

void StatComponent::Init() {
  if (std::shared_ptr<GameObject> ownerPtr = owner.lock()) {
    if (!ownerPtr->HasComponent<SpeciesComponent>()) {
      throw std::runtime_error(
          "PokemonStatsComponent requires a PokemonSpeciesComponent.");
    }
    m_species = &ownerPtr->GetComponent<SpeciesComponent>();
  } else {
    throw std::runtime_error("PokemonStatsComponent has no owner during Init.");
  }

  CalculateStats();
}

void StatComponent::CalculateStats() {
  if (!m_species) {
    spdlog::error(
        "Cannot calculate stats, PokemonSpeciesComponent is missing!");
    return;
  }

  maxHp = m_species->baseStats.hp + (level * 5);
  attack = m_species->baseStats.attack + (level * 2);
  defense = m_species->baseStats.defense + (level * 2);
  currentHp = maxHp;

  spdlog::info("{} at level {} has {} HP, {} ATK.", m_species->speciesName,
               level, maxHp, attack);
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
