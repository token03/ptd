#pragma once

#include "data/PMDData.h"
#include <filesystem>
#include <optional>
#include <string>

namespace PMDUtils {

std::optional<AnimationData>
parseAnimationData(const std::filesystem::path &xmlPath);

std::string findAnimationBaseName(const PMDData &form,
                                  const std::string &animationName);

std::string generateFullName(const std::string &formattedParentName,
                             const std::string &unformattedEntryName);

std::string generateFullId(const std::string &dex,
                           const std::filesystem::path &relativePath);

} // namespace PMDUtils
