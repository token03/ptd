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

file(GLOB_RECURSE PTD_SOURCES
  "src/main.cpp"
  "src/impl.cpp"
  "src/core/*.cpp"
  "src/components/*.cpp"
  "src/factories/*.cpp"
  "src/utils/*.cpp"
  "src/data/*.cpp"
  "src/managers/*.cpp"
  "src/scenes/*.cpp"
  "src/ui/*.cpp"
  "src/tools/*.cpp"
)

add_executable(ptd ${PTD_SOURCES})

raylib_imgui_setup_dependencies(ptd)

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
target_include_directories(ptd PUBLIC ${PROJECT_SOURCE_DIR}/src)
