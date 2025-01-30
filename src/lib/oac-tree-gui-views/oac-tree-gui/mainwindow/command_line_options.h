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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_COMMAND_LINE_OPTIONS_H_
#define SEQUENCERGUI_MAINWINDOW_COMMAND_LINE_OPTIONS_H_

#include <QSize>
#include <QString>
#include <optional>

namespace oac_tree_gui
{

/**
 * @brief The Options struct contains the result of command line option parsing.
 */
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

  //! initial window size
  std::optional<QSize> window_size;
};

/**
 * @brief Parses command line options and returns parsing result.
 */
Options ParseOptions(int argc, char** argv);

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_MAINWINDOW_COMMAND_LINE_OPTIONS_H_
