/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
#define SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_

//! Collection of helper methods common for all main windows.

#include <string>
#include <vector>

namespace sequencergui
{

/**
 * @brief Returns vector of names representing sequencer procedures located in a given folder.
 *
 * @param path The name of the folder, or the file name itself.
 *
 * @details If given path is the procedure itself, will return a vector with this name as a single
 * element.
 */
std::vector<std::string> GetProcedureFiles(const std::string& path_name);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
