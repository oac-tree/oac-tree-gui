# Project       : Graphical User Interface for SUP Sequencer
#
# Description   : Integrated development environment for Sequencer procedures
#
# Author        : Gennady Pospelov (IO)
#
# Copyright (c) : 2010-2023 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.

# Set required CMake version
# This sets the default CMake behaviour through policies allowing compatibility with future versions
# Justification:
#   - CMake 3.12 - CMP0074: find_package command now searches prefixes specified by the <PackageName>_ROOT environment variable
#       CODAC systems contain conflicting environment variables which generate warnings when this policy is not set to NEW
#   - CMake 3.13 - CMP0076: relative source file paths are interpreted as being relative to the current source directory
cmake_minimum_required(VERSION 3.13)

project(sequencer-gui VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(COA_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(COA_USE_QT6 "Compile with Qt6" OFF)
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
