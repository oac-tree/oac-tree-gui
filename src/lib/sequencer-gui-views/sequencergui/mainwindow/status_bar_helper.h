/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_STATUS_BAR_HELPER_H_
#define SEQUENCERGUI_MAINWINDOW_STATUS_BAR_HELPER_H_

//! @file
//! Collection of helper functions for main QStatusBar.

#include <QString>

class QToolButton;

namespace sequencergui
{

/**
 * @brief Setup status bar button.
 *
 * It will tune tool button appearance intended for the main application status bar, and it will
 * connects the button with proxy action.
 *
 * @param button The button to setup.
 * @param command_id The id of the command corresponding to the action in the main menubar.
 */
void SetupStatusBarButton(QToolButton* button, const QString& command_id);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_STATUS_BAR_HELPER_H_
