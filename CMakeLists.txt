cmake_minimum_required(VERSION 3.28)
project(SFML_First_Game LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output binary files to bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

option(USE_FETCH "Use FetchContent to downlaod SFML/ImGui" OFF)
#Option 1: Use Fetch to auto download from SFML Github 
#(Better when wanting to share code)
#========================================
if(USE_FETCH)
    include(FetchContent)
    FetchContent_Declare(
    SFML
     GIT_REPOSITORY https://github.com/SFML/SFML.git
     GIT_TAG 3.0.2
     GIT_SHALLOW ON
     EXCLUDE_FROM_ALL
    SYSTEM
)
 FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
imgui 
GIT_REPOSITORY https://github.com/ocornut/imgui.git
GIT_TAG v1.91.1
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "Path to ImGui root (contains imgui.h)" FORCE)
 FetchContent_Declare(
 ImGui_SFML
 GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
 GIT_TAG v3.0
 )
 FetchContent_MakeAvailable(ImGui_SFML)

#=========================================

else()
# Option 2: System installed SFML (Better for Local use only)
#=======================================================================
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Window System)
set(IMGUI_DIR "/home/patri/imgui" CACHE PATH "Path to ImGui root (contains imgui.h)" FORCE)
add_subdirectory("/home/patri/imgui-sfml" ${CMAKE_BINARY_DIR}/imgui-sfml-build)
endif()
#=====================================================================

# Option 2: Use Fetch to auto download from SFML Github 
#(Better when wanting to share code)
#========================================
# include(FetchContent)
# FetchContent_Declare(
# SFML
# GIT_REPOSITORY https://github.com/SFML/SFML.git
# GIT_TAG 3.0.2
# GIT_SHALLOW ON
# EXCLUDE_FROM_ALL
# SYSTEM
# )
# FetchContent_MakeAvailable(SFML)
# FetchContent_Declare(
# ImGui-SFML
# GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
# GIT_TAG master
# )
# FetchContent_MakeAvailable(ImGui_SFML)
#=========================================


file(GLOB SOURCES src/*.cpp)

add_executable(SFML_First_Game src/main.cpp)

target_link_libraries(SFML_First_Game PRIVATE 
SFML::Graphics 
SFML::Audio 
SFML::Window 
SFML::System
ImGui-SFML::ImGui-SFML
)

