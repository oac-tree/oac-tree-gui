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

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_SETTINGS_MANAGER_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_SETTINGS_MANAGER_H_

#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>

namespace oac_tree_gui::test
{

/**
 * @brief The TestSettingsManager class is a settings manager with in-memory application storage.
 */
class TestSettingsManager : public sup::gui::SettingsManager
{
public:
  TestSettingsManager();

private:
  sup::gui::InMemorySettingsStorage m_storage;
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_SETTINGS_MANAGER_H_
