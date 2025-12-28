cmake_minimum_required(VERSION 3.28)
project(SFML_First_Game LANGUAGES CXX)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# C++ standard
# (Use C++23 here to keep SFML happy on macOS/Clang while still being "modern C++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Put the exe into bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ------------------------------
# Dependencies via FetchContent
# ------------------------------
include(FetchContent)

# Versions
set(SFML_VERSION 3.0.2)
set(IMGUI_VERSION v1.91.1)
set(IMGUISFML_VERSION v3.0)

# Static libs (good for shipping to friends)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# SFML options – trim stuff we don't need
set(SFML_BUILD_AUDIO ON  CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

# --- SFML ---
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        ${SFML_VERSION}
    GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(SFML)

# --- Dear ImGui (core) ---
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        ${IMGUI_VERSION}
    GIT_SHALLOW    ON
)

# --- ImGui-SFML backend (no CMake targets, we just use the .cpp directly) ---
FetchContent_Declare(
    imgui_sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG        ${IMGUISFML_VERSION}
    GIT_SHALLOW    ON
)

# We want the sources, not their CMake project
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

FetchContent_GetProperties(imgui_sfml)
if(NOT imgui_sfml_POPULATED)
    FetchContent_Populate(imgui_sfml)
endif()

# ------------------------------
# Game sources
# ------------------------------
file(GLOB_RECURSE GAME_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(SFML_First_Game
    ${GAME_SOURCES}

    # ImGui core
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    # optional, if you want the demo window:
    # ${imgui_SOURCE_DIR}/imgui_demo.cpp

    # ImGui-SFML glue
    ${imgui_sfml_SOURCE_DIR}/imgui-SFML.cpp
)

# -----------------------------------------
# CONFIG / ASSETS / GAME DATA COPY (SAFE)
# -----------------------------------------

# 1) config.txt – only copy if it exists
set(GAME_CONFIG_FILE "${CMAKE_SOURCE_DIR}/config.txt")

if(EXISTS "${GAME_CONFIG_FILE}")
    add_custom_command(
        TARGET SFML_First_Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GAME_CONFIG_FILE}"
                "$<TARGET_FILE_DIR:SFML_First_Game>/config.txt"
        COMMENT "Copying config.txt to output directory"
    )
else()
    message(STATUS "No config.txt at ${GAME_CONFIG_FILE} – skipping config copy.")
endif()

# 2) assets/ folder – only copy if it exists
set(GAME_ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

if(EXISTS "${GAME_ASSETS_DIR}")
    add_custom_command(
        TARGET SFML_First_Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GAME_ASSETS_DIR}"
                "$<TARGET_FILE_DIR:SFML_First_Game>/assets"
        COMMENT "Copying assets folder to output directory"
    )
else()
    message(STATUS "No 'assets' folder at ${GAME_ASSETS_DIR} – skipping asset copy.")
endif()

# 3) game_data/ folder – for future .txt levels/config
set(GAME_DATA_DIR "${CMAKE_SOURCE_DIR}/game_data")

if(EXISTS "${GAME_DATA_DIR}")
    add_custom_command(
        TARGET SFML_First_Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GAME_DATA_DIR}"
                "$<TARGET_FILE_DIR:SFML_First_Game>/game_data"
        COMMENT "Copying game_data folder (txt levels/config) to output directory"
    )
else()
    message(STATUS "No 'game_data' folder at ${GAME_DATA_DIR} – skipping game data copy.")
endif()

# Include dirs: your own + ImGui + ImGui-SFML
target_include_directories(SFML_First_Game PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR}
    ${imgui_sfml_SOURCE_DIR}
)

# Link only SFML libraries – ImGui code is compiled into the exe
target_link_libraries(SFML_First_Game PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Window
    SFML::System
)

# ------------------------------
# Warnings
# ------------------------------
if(MSVC)
    target_compile_options(SFML_First_Game PRIVATE /W4 /permissive-)
else()
    target_compile_options(SFML_First_Game PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wnon-virtual-dtor
        -Woverloaded-virtual
    )
endif()

# ------------------------------
# Optional sanitizers (Linux/Clang/GCC)
# ------------------------------
option(ENABLE_SANITIZERS "Enable Address/UB sanitizers in Debug" OFF)

if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(SFML_First_Game PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )
        target_link_options(SFML_First_Game PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )
    endif()
endif()
