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

#ifndef OAC_TREE_GUI_MODEL_SEQUENCER_SETTINGS_HELPER_H_
#define OAC_TREE_GUI_MODEL_SEQUENCER_SETTINGS_HELPER_H_

#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief Returns the vector of strings representing the list of directories where to look for
 * plugins.
 *
 * On the CODAC system, this function will report a single "/opt/codac/lib/oac-tree/plugins" record.
 * The plugin load machinery will then try to load all plugins found in this folder.
 *
 * On non-CODAC, the result will be empty. The plugin load machinery will rely on
 * GetDefaultPluginList().
 */
std::vector<std::string> GetDefaultPluginDirList();

/**
 * @brief Returns the default list of plugin names to load.
 *
 * On the CODAC system, the result will be empty. The plugin load machinery will rely on
 * GetDefaultPluginDirList() On non-CODAC systems, the result will contain a list of plugins that
 * usually come with oac-tree bundle.
 *
 * @details The plugin name contains no prefix "lib" and no suffixes ".so" or ".dylib".
 */
std::vector<std::string> GetDefaultPluginList();

/**
 * @brief Finds shared libraries in the given folder.
 */
std::vector<std::string> FindSharedLibraries(const std::string& dir);
}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_SEQUENCER_SETTINGS_HELPER_H_
