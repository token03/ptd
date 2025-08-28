include(cmake/CPM.cmake)

function(raylib_imgui_setup_dependencies)
  message(STATUS "Adding Dear ImGui v1.92.1")
  CPMAddPackage(
    NAME ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG        v1.92.1
  )

  message(STATUS "Adding raylib v5.5")
  CPMAddPackage(
    NAME raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib
    GIT_TAG        5.5
  )

  message(STATUS "Adding spdlog v1.15.3")
  CPMAddPackage(
    NAME spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        v1.15.3
  )

  message(STATUS "Adding pugixml v1.15")
  CPMAddPackage(
    NAME pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml
    GIT_TAG        v1.15
  )

  message(STATUS "Adding glaze v5.6.1")
  CPMAddPackage(
    NAME glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze
    GIT_TAG        v5.6.1
  )

  message(STATUS "Adding rlImGui")
  CPMAddPackage(
    NAME rlImGui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui
    GIT_TAG        main
  )

  add_library(imgui STATIC
    ${ImGui_SOURCE_DIR}/imgui.cpp
    ${ImGui_SOURCE_DIR}/imgui_draw.cpp
    ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
    ${ImGui_SOURCE_DIR}/imgui_tables.cpp
  )

  add_library(glaze INTERFACE)
  target_include_directories(glaze INTERFACE ${glaze_SOURCE_DIR}/include)

  target_include_directories(imgui PUBLIC ${ImGui_SOURCE_DIR})

  add_library(rlimgui STATIC
    ${rlImGui_SOURCE_DIR}/rlImGui.cpp
  )

  target_include_directories(rlimgui PUBLIC
    ${rlImGui_SOURCE_DIR}
    ${ImGui_SOURCE_DIR}
  )

  target_link_libraries(rlimgui PUBLIC
    imgui
    raylib
  )

endfunction()
