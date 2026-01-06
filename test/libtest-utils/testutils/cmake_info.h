/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_CMAKE_INFO_H_
#define LIBTEST_UTILS_TESTUTILS_CMAKE_INFO_H_

//! @file
//! Collection of helper methosd to retrieve CMake build time information.

#include <string>

namespace oac_tree_gui::test
{

/**
 * @brief Returns the full path to the source directory.
 */
std::string CMakeSourceDir();

/**
 * @brief Returns full path to the build directory.
 */
std::string CMakeBinaryDir();

/**
 * @brief Returns full path to the folder with test results.
 *
 * Normally points to CMAKE_CURRENT_BINARY_DIR/test_output
 */
std::string TestOutputDir();

/**
 * @brief Returns full path to the project source directory.
 *
 * Can be different from CMake source directory, if the project is in a submodule.
 */
std::string ProjectSourceDir();

/**
 * @brief Returns full path to resource folder.
 */
std::string ProjectResourceDir();

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_CMAKE_INFO_H_
