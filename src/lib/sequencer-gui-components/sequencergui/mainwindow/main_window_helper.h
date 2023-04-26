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

//! Collection of utils for main() function.

#include <QString>

namespace sequencergui
{

//! Sets main GUI style.
void SetWindowStyle(const QString& user_style);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
