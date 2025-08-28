cmake_minimum_required(VERSION 3.16)
project(ptd LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
add_library(raylib_imgui_compiler_flags INTERFACE)
target_compile_features(raylib_imgui_compiler_flags INTERFACE cxx_std_17)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
    raylib_imgui_compiler_flags
    INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-pedantic-errors;-Werror;-Wall;-Weffc++;-Wextra;-Wconversion;-Wsign-conversion>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W4>>")

# include(cmake/StaticAnalysers.cmake)
# enable_clang_tidy()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

include(Dependencies.cmake)
raylib_imgui_setup_dependencies()

add_executable(ptd
  src/main.cpp
)
set_target_properties(ptd PROPERTIES CXX_CLANG_TIDY
                                                "${CLANG_TIDY_COMMAND}")
target_link_libraries(
    ptd
    PUBLIC imgui
           raylib
           rlimgui
           spdlog::spdlog_header_only
           raylib_imgui_compiler_flags)
target_compile_definitions(ptd PRIVATE SPDLOG_FMT_EXTERNAL)
target_compile_definitions(
    ptd PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
target_include_directories(ptd PUBLIC "${PROJECT_SOURCE_DIR}/src")
