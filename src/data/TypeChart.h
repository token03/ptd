#pragma once

#include "Enums.h"
#include "TypeData.h"
#include <unordered_map>
#include <vector>

class TypeChart {
public:
  TypeChart(const std::vector<TypeData> &allTypeData) {
    for (const auto &typeData : allTypeData) {
      for (const auto &eff : typeData.effectiveness) {
        m_chart[typeData.type][eff.against] = static_cast<float>(eff.value);
      }
    }
  }

  float getEffectiveness(PokemonType attackType, PokemonType targetType) const {
    if (auto it = m_chart.find(attackType); it != m_chart.end()) {
      if (auto jt = it->second.find(targetType); jt != it->second.end()) {
        return jt->second;
      }
    }
    return 1.0f;
  }

private:
  std::unordered_map<PokemonType, std::unordered_map<PokemonType, float>>
      m_chart;
};
