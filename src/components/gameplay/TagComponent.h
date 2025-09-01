#pragma once

#include <string>

#include "core/Component.h"

class TagComponent : public Component {
 public:
  std::string tag;
  TagComponent(std::string tag) : tag(std::move(tag)) {}
};
