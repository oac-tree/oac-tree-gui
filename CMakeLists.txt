cmake_minimum_required(VERSION 3.13.5)
project(sequencer-gui VERSION 0.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(SEQUENCERGUI_BUMP_VERSION "Propagate version number" OFF)
option(SEQUENCERGUI_CODAC "Build using dependencies from the CODAC environment" OFF)
option(SEQUENCERGUI_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(SEQUENCERGUI_USE_QT6 "Compile with Qt6" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
include(configuration)

add_subdirectory(source)
add_subdirectory(tests)

# Defines 'clangformat' target. See CodeTools.cmake for details.
project_codetools_setup()
