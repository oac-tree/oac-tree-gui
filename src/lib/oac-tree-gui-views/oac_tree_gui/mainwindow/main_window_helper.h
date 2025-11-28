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

#ifndef OAC_TREE_GUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
#define OAC_TREE_GUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_

//! @file
//! Collection of helper methods common for all sequencer main windows.

#include <QString>
#include <functional>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief Opens a message box with the question if running jobs should be stopped.
 */
bool ShouldStopRunningJobs();

/**
 * @brief Returns vector of names representing sequencer procedures located in a given folder.
 *
 * @param path The name of the folder, or the file name itself.
 *
 * @details If given path is the procedure itself, will return a vector with this name as a single
 * element.
 */

std::vector<std::string> GetProcedureFiles(const std::string& path_name);

/**
 * @brief Loads procedures, print warnings if something went wrong.
 *
 * @param file_name The name of the file or directory to load procedures.
 * @param func A function to call on every procedure found.
 */
void ImportProcedures(const QString& file_name,
                      const std::function<bool(const QString& name)>& func);

/**
 * @brief Returns the name of the Sequencer XML procedure file as selected by the user.
 *
 * Will invoke file selection dialog and return user choice. The name will be empty if the user has
 * canceled the dialog. The last working directory will be saved in the persistence settings under
 * the given key (if non-empty), so the next time dialog will be opened there.
 *
 * @param key The key to store the setting in the global application persistent setting file.
 */
QString GetOpenSequencerProcedureName(const QString& key = {});

/**
 * @brief Returns a callback to convert object name to corresponding plugin name.
 *
 * This method requires that the MainWindow exists and is properly wired.
 */
std::function<std::string(const std::string&)> CreatePluginNameCallback();

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
