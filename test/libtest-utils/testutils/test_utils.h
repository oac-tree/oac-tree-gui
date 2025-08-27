/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_

//! @file
//! Collection of utility functions for various unit tests.

#include <chrono>
#include <functional>
#include <string>

namespace oac_tree_gui::test
{

/**
 * @brief Wait for timeout in milisseconds or until predicate returns true.
 */
bool WaitFor(std::function<bool()> predicate, std::chrono::milliseconds timeout);

/**
 * @brief Converts milliseconds in timeout as expected by Wait instruction.
 */
double GetTimeoutInSec(std::chrono::milliseconds timeout);

/**
 * @brief Creates oac-tree XML procedure by wrapping the body into necessary Procedure directive.
 */
std::string CreateProcedureString(const std::string& body, bool schema = true);

/**
 * @brief Checks if vectors are of the same size and contain the same elements (order is not
 * important).
 */
bool HaveSameElements(const std::vector<std::string>& vec1, const std::vector<std::string>& vec2);

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_
