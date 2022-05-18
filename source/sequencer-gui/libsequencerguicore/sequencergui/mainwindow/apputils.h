/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_APPUTILS_H
#define SEQUENCERGUI_MAINWINDOW_APPUTILS_H

//! @file apputils.h
//! Collection of utils for mainwindow.

#include <string>

namespace sequencergui
{

//! Returns multiline-string describing system environment related to the user desktop.
std::string GetDesktopInfo();


}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_APPSETTINGS_H
