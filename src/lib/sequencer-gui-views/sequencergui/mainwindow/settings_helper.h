/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_SETTINGS_HELPER_H_
#define SEQUENCERGUI_MAINWINDOW_SETTINGS_HELPER_H_

//!< Collection of helper function to access global application settings.

namespace sequencergui
{

class SettingsModel;

/**
 * @brief Returns model with global application settings.
 */
const SettingsModel& GetGlobalSettings();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SETTINGS_HELPER_H_
