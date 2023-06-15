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

#ifndef SEQUENCERGUI_MAINWINDOW_COMMAND_LINE_OPTIONS_H_
#define SEQUENCERGUI_MAINWINDOW_COMMAND_LINE_OPTIONS_H_

//! Collection of utils for main() function.

#include <QString>

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

  //! main appplication GUI style
  QString style;

  //! the name of sequencer procedure to open
  QString file_name;
};

//! Parse command line options.
Options ParseOptions(int argc, char** argv);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_COMMAND_LINE_OPTIONS_H_
