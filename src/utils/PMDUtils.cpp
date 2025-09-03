#include "utils/PMDUtils.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cctype>

#include "pugixml.hpp"

namespace PMDUtils {

std::optional<AnimationData> parseAnimationData(const std::filesystem::path &xmlPath) {
  if (!std::filesystem::exists(xmlPath)) {
    return std::nullopt;
  }
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());
  if (!result) {
    spdlog::warn("Failed to parse XML {}: {}", xmlPath.string(), result.description());
    return std::nullopt;
  }

  AnimationData data;
  pugi::xml_node animDataNode = doc.child("AnimData");
  if (!animDataNode) return std::nullopt;

  data.shadowSize = animDataNode.child("ShadowSize").text().as_int(1);

  for (pugi::xml_node animNode : animDataNode.child("Anims").children("Anim")) {
    Animation anim;
    anim.name = animNode.child("Name").text().as_string();

    if (pugi::xml_node copyNode = animNode.child("CopyOf")) {
      anim.copyOf = copyNode.text().as_string();
    } else {
      anim.index = animNode.child("Index").text().as_int();
      anim.frameWidth = animNode.child("FrameWidth").text().as_int();
      anim.frameHeight = animNode.child("FrameHeight").text().as_int();
      if (auto node = animNode.child("RushFrame")) anim.rushFrame = node.text().as_int();
      if (auto node = animNode.child("HitFrame")) anim.hitFrame = node.text().as_int();
      if (auto node = animNode.child("ReturnFrame"))
        anim.returnFrame = node.text().as_int();
      for (pugi::xml_node durationNode :
           animNode.child("Durations").children("Duration")) {
        anim.durations.push_back(durationNode.text().as_int());
      }
    }
    data.animations[anim.name] = anim;
  }

  std::vector<std::string> copyKeys;
  for (const auto &[name, anim] : data.animations) {
    if (anim.copyOf) copyKeys.push_back(name);
  }

  for (const auto &key : copyKeys) {
    auto &animToModify = data.animations.at(key);
    if (auto it = data.animations.find(*animToModify.copyOf);
        it != data.animations.end()) {
      const auto &sourceAnim = it->second;
      animToModify = sourceAnim;
      animToModify.name = key;
      animToModify.copyOf = *animToModify.copyOf;
    }
  }

  return data;
}

std::string findAnimationBaseName(const PMDData &form, const std::string &animationName) {
  std::string baseName;
  size_t longestMatch = 0;
  for (const auto &potentialBase : form.animFileBases) {
    if (animationName.rfind(potentialBase, 0) == 0 &&
        potentialBase.length() > longestMatch) {
      baseName = potentialBase;
      longestMatch = potentialBase.length();
    }
  }
  return baseName;
}

std::vector<std::string> findAnimationBases(const std::filesystem::path &spriteDir) {
  std::vector<std::string> bases;
  if (!std::filesystem::exists(spriteDir) || !std::filesystem::is_directory(spriteDir)) {
    return bases;
  }
  const std::string animSuffix = "-Anim.png";
  for (const auto &dir_entry : std::filesystem::directory_iterator{spriteDir}) {
    if (dir_entry.is_regular_file()) {
      std::string filename = dir_entry.path().filename().string();
      if (filename.ends_with(animSuffix)) {
        bases.push_back(filename.substr(0, filename.length() - animSuffix.length()));
      }
    }
  }
  return bases;
}

std::set<std::string> findAvailablePortraits(const std::filesystem::path &portraitDir) {
  std::set<std::string> portraits;
  if (!std::filesystem::exists(portraitDir) ||
      !std::filesystem::is_directory(portraitDir)) {
    return portraits;
  }
  for (const auto &dir_entry : std::filesystem::directory_iterator{portraitDir}) {
    if (dir_entry.is_regular_file() && dir_entry.path().extension() == ".png") {
      portraits.insert(dir_entry.path().stem().string());
    }
  }
  return portraits;
}

namespace {
std::string formatFormNameComponent(const std::string &s) {
  size_t start = s.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) return std::string{};
  size_t end = s.find_last_not_of(" \t\r\n");
  std::string sub = s.substr(start, end - start + 1);
  std::string lower;
  lower.reserve(sub.size());
  for (unsigned char c : sub) lower.push_back(static_cast<char>(std::tolower(c)));
  std::string res;
  bool lastHyphen = false;
  for (unsigned char c : lower) {
    if (std::isspace(c) || c == '/' || c == '\\') {
      if (!lastHyphen) {
        res.push_back('-');
        lastHyphen = true;
      }
    } else {
      res.push_back(static_cast<char>(c));
      lastHyphen = false;
    }
  }
  if (!res.empty() && res.front() == '-') res.erase(res.begin());
  if (!res.empty() && res.back() == '-') res.pop_back();
  return res;
}
}  // namespace

std::string generateFullName(const std::string &formattedParentName,
                             const std::string &unformattedEntryName) {
  std::string ecomp = formatFormNameComponent(unformattedEntryName);
  if (formattedParentName.empty()) return ecomp;
  if (ecomp.empty()) return formattedParentName;
  return formattedParentName + "-" + ecomp;
}

std::string generateFullId(const std::string &dex,
                           const std::filesystem::path &relativePath) {
  std::string path_str = relativePath.string();
  if (path_str.empty()) {
    return dex;
  }

  std::replace(path_str.begin(), path_str.end(), '\\', '/');

  if (path_str.front() == '/') {
    path_str = path_str.substr(1);
  }

  std::replace(path_str.begin(), path_str.end(), '/', '-');

  return dex + "-" + path_str;
}

}  // namespace PMDUtils
