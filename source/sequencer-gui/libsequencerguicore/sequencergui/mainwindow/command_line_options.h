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

#ifndef SEQUENCERGUI_MAINWINDOW_APP_UTILS_H_
#define SEQUENCERGUI_MAINWINDOW_APP_UTILS_H_

//! @file app_utils.h
//! Collection of utils for main() function.

#include <memory>
#include <string>

namespace sequencergui
{

struct Options
{
  //! rely on system scale via QT_ variables, if true
  bool scale = false;

  //! print system environment information
  bool info = false;

  //! appplication system font point size
  int system_font_psize = -1;
};

//! Parse command line options.
Options ParseOptions(int argc, char** argv);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_APP_UTILS_H_
