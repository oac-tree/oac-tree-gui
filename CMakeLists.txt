cmake_minimum_required(VERSION 3.13.5)
project(sequencer-gui VERSION 0.4.99 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(SEQUENCERGUI_BUMP_VERSION "Propagate version number" OFF)
option(SEQUENCERGUI_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(SEQUENCERGUI_USE_QT6 "Compile with Qt6" OFF)
option(COA_BUILD_DOCUMENTATION "Build documentation" OFF)
option(COA_NO_CODAC "Don't look for the presence of CODAC environment" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
set(CMAKE_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/configs)
include(configuration)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)

# Defines 'clangformat' target. See CodeTools.cmake for details.
project_codetools_setup()
