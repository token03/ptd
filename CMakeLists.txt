cmake_minimum_required(VERSION 3.16)
project(ptd LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_WARNINGS_AS_ERRORS True)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(raylib_imgui_compiler_flags INTERFACE)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# include(cmake/StaticAnalysers.cmake)
# enable_clang_tidy()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

include(Dependencies.cmake)
raylib_imgui_setup_dependencies()

add_executable(ptd
  src/main.cpp
  src/core/Game.cpp
  src/loaders/PMDLoader.cpp
  src/loaders/PokemonDataLoader.cpp
  src/components/AnimationComponent.cpp
  src/components/SpriteComponent.cpp
  src/components/SpeciesComponent.cpp
  src/components/StatComponent.cpp
  src/components/TransformComponent.cpp
  src/components/PathComponent.cpp
  src/components/PathFollowerComponent.cpp
  src/components/PersonalityComponent.cpp
  src/factories/PokemonFactory.cpp
  src/utils/Debugger.cpp
)
set_target_properties(ptd PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
target_link_libraries(
    ptd
    PUBLIC  imgui
            raylib
            rlimgui
            pugixml
            glaze::glaze
            spdlog
            raylib_imgui_compiler_flags)
target_compile_definitions(
    ptd PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
target_include_directories(ptd PUBLIC "${PROJECT_SOURCE_DIR}/src")
