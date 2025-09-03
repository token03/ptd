#pragma once

#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "data/PMDData.h"

namespace PMDUtils {

std::optional<AnimationData> parseAnimationData(const std::filesystem::path &xmlPath);

std::string findAnimationBaseName(const PMDData &form, const std::string &animationName);

std::vector<std::string> findAnimationBases(const std::filesystem::path &spriteDir);

std::set<std::string> findAvailablePortraits(const std::filesystem::path &portraitDir);

std::string generateFullName(const std::string &formattedParentName,
                             const std::string &unformattedEntryName);

std::string generateFullId(const std::string &dex,
                           const std::filesystem::path &relativePath);

}  // namespace PMDUtils
