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

#ifndef OAC_TREE_GUI_MODEL_SEQUENCER_SETTINGS_HELPER_H
#define OAC_TREE_GUI_MODEL_SEQUENCER_SETTINGS_HELPER_H

#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief Returns the vector of strings representing the list of directories where to look for
 * plugins.
 */
std::vector<std::string> GetDefaultPluginDirList();

/**
 * @brief Finds shared libraries in the given folder.
 */
std::vector<std::string> FindSharedLibraries(const std::string& dir);
}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_SEQUENCER_SETTINGS_HELPER_H
