#include "utils/PMDLoader.h"
#include "components/AnimationComponent.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"
#include <algorithm>
#include <iostream>

PMDLoader::PMDLoader(const std::filesystem::path &assetRoot)
    : m_assetRoot(assetRoot), m_pmdCollabPath(assetRoot / "pmdcollab"),
      m_portraitPath(assetRoot / "pmdcollab" / "portrait") {}

PMDLoader::~PMDLoader() {
  for (auto const &[path, texture] : m_textureCache) {
    UnloadTexture(texture);
  }
  m_textureCache.clear();
}

bool PMDLoader::loadPokemon(const std::string &dexNumber) {
  if (!m_trackerLoaded) {
    const auto trackerPath = m_pmdCollabPath / "tracker.json";
    std::cout << "Loading tracker file: " << trackerPath << std::endl;
    std::string buffer;
    auto error = glz::read_file_json<glz::opts{.error_on_unknown_keys = false}>(
        m_trackerData, trackerPath.string(), buffer);
    if (error) {
      std::cerr << "Failed to parse tracker.json: "
                << glz::format_error(error, buffer) << std::endl;
      return false;
    }
    m_trackerLoaded = true;
    std::cout << "Tracker.json loaded successfully." << std::endl;
  }

  auto it = m_trackerData.find(dexNumber);
  if (it == m_trackerData.end()) {
    std::cerr << "Could not find Pokémon with dex #" << dexNumber
              << " in tracker.json" << std::endl;
    return false;
  }

  const TrackerEntry &baseEntry = it->second;
  processTrackerEntry(dexNumber, "", baseEntry, "", std::filesystem::path());
  return true;
}

void PMDLoader::processTrackerEntry(const std::string &dex,
                                    const std::string &subgroupId,
                                    const TrackerEntry &entry,
                                    const std::string &parentName,
                                    const std::filesystem::path &parentPath) {
  // Construct name, ID, and path
  std::string currentFullName =
      parentName.empty() ? entry.name : parentName + " " + entry.name;
  std::filesystem::path currentRelativePath = parentPath / subgroupId;

  std::string path_str = currentRelativePath.string();
  if (!path_str.empty() &&
      (path_str.front() == '/' || path_str.front() == '\\')) {
    path_str = path_str.substr(1);
  }
  std::replace(path_str.begin(), path_str.end(), '/', '-');
  std::replace(path_str.begin(), path_str.end(), '\\', '-');

  std::string currentFullId = dex;
  if (!path_str.empty()) {
    currentFullId += "-" + path_str;
  }

  PokemonForm newForm;
  newForm.dex = dex;
  newForm.fullId = currentFullId;
  newForm.fullName = currentFullName;
  newForm.formPath = currentRelativePath.string();
  newForm.spriteCredit = entry.sprite_credit;
  newForm.portraitCredit = entry.portrait_credit;

  std::filesystem::path animDataPath =
      m_pmdCollabPath / "sprite" / dex / currentRelativePath / "AnimData.xml";
  newForm.animData = parseAnimationData(animDataPath);

  std::filesystem::path spritePath =
      m_pmdCollabPath / "sprite" / dex / currentRelativePath;
  if (std::filesystem::exists(spritePath) &&
      std::filesystem::is_directory(spritePath)) {
    const std::string animSuffix = "-Anim.png";
    for (const auto &dir_entry :
         std::filesystem::directory_iterator{spritePath}) {
      if (dir_entry.is_regular_file()) {
        std::string filename = dir_entry.path().filename().string();
        if (filename.ends_with(animSuffix)) {
          std::string baseName =
              filename.substr(0, filename.length() - animSuffix.length());
          newForm.animFileBases.push_back(baseName);
        }
      }
    }
  }

  std::filesystem::path portraitFormPath =
      m_portraitPath / dex / currentRelativePath;

  if (std::filesystem::exists(portraitFormPath) &&
      std::filesystem::is_directory(portraitFormPath)) {
    for (const auto &dir_entry :
         std::filesystem::directory_iterator{portraitFormPath}) {
      if (dir_entry.is_regular_file() &&
          dir_entry.path().extension() == ".png") {
        newForm.availablePortraits.insert(dir_entry.path().stem().string());
      }
    }
  }

  if (newForm.animData || !newForm.availablePortraits.empty()) {
    m_loadedForms[currentFullId] = newForm;
    std::cout << "Successfully loaded form: " << newForm.fullName
              << " (ID: " << newForm.fullId << ")" << std::endl;
  }

  for (const auto &[id, subEntry] : entry.subgroups) {
    processTrackerEntry(dex, id, subEntry, currentFullName,
                        currentRelativePath);
  }
}

std::optional<AnimationData>
PMDLoader::parseAnimationData(const std::filesystem::path &xmlPath) {
  if (!std::filesystem::exists(xmlPath)) {
    return std::nullopt;
  }
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());
  if (!result) {
    std::cerr << "XML [" << xmlPath
              << "] parsed with errors: " << result.description() << std::endl;
    return std::nullopt;
  }

  AnimationData data;
  pugi::xml_node animDataNode = doc.child("AnimData");
  if (!animDataNode)
    return std::nullopt;

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
      if (auto node = animNode.child("RushFrame"))
        anim.rushFrame = node.text().as_int();
      if (auto node = animNode.child("HitFrame"))
        anim.hitFrame = node.text().as_int();
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
    if (anim.copyOf)
      copyKeys.push_back(name);
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

const PokemonForm *PMDLoader::getForm(const std::string &fullId) const {
  auto it = m_loadedForms.find(fullId);
  return (it != m_loadedForms.end()) ? &it->second : nullptr;
}

std::shared_ptr<GameObject>
PMDLoader::CreatePokemonObject(const std::string &formId,
                               const std::string &initialAnimation,
                               Vector2 position, Vector2 scale) {
  const PokemonForm *form = getForm(formId);
  if (!form || !form->animData) {
    std::cerr << "Could not create GameObject for form ID: " << formId
              << ". Form data not loaded or has no animations." << std::endl;
    return nullptr;
  }

  Texture2D texture = getAnimationTexture(formId, initialAnimation);
  if (texture.id <= 0) {
    std::cerr << "Could not load initial texture for animation: "
              << initialAnimation << std::endl;
    return nullptr;
  }

  auto gameObject = std::make_shared<GameObject>();

  auto &transform = gameObject->AddComponent<TransformComponent>();
  transform.position = position;
  transform.scale = scale;

  gameObject->AddComponent<SpriteComponent>(texture);
  gameObject->AddComponent<AnimationComponent>(this, formId);

  gameObject->GetComponent<AnimationComponent>().Play(initialAnimation);

  return gameObject;
}

std::string PMDLoader::findAnimationBaseName(const PokemonForm &form,
                                             const std::string &animationName) {
  std::string baseName;
  size_t longestMatch = 0;
  for (const auto &potentialBase : form.animFileBases) {
    if (animationName.rfind(potentialBase, 0) == 0 &&
        potentialBase.length() > longestMatch) { // starts_with
      baseName = potentialBase;
      longestMatch = potentialBase.length();
    }
  }
  return baseName;
}

Texture2D PMDLoader::getAnimationTexture(const std::string &formId,
                                         const std::string &animationName) {
  const PokemonForm *form = getForm(formId);
  if (!form) {
    std::cerr << "Form not found for ID: " << formId << std::endl;
    return Texture2D{0};
  }

  std::string baseName = findAnimationBaseName(*form, animationName);

  if (baseName.empty()) {
    std::cerr << "Could not determine texture file for animation '"
              << animationName << "' in form '" << form->fullName << "'"
              << std::endl;
    return Texture2D{0};
  }

  std::filesystem::path texturePath = m_pmdCollabPath / "sprite" / form->dex /
                                      form->formPath / (baseName + "-Anim.png");

  std::string pathString = texturePath.string();

  if (m_textureCache.count(pathString)) {
    return m_textureCache.at(pathString);
  }

  if (!std::filesystem::exists(texturePath)) {
    std::cerr << "Texture file not found: " << pathString << std::endl;
    return Texture2D{0};
  }

  Texture2D texture = LoadTexture(pathString.c_str());
  if (texture.id > 0) {
    std::cout << "Loaded texture: " << pathString << std::endl;
    m_textureCache[pathString] = texture;
  } else {
    std::cerr << "Failed to load texture with raylib: " << pathString
              << std::endl;
  }
  return texture;
}
