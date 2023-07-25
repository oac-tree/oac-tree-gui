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

#include "command_line_options.h"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>

namespace sequencergui
{
Options ParseOptions(int argc, char** argv)
{
  // Parser requires an application to be created upfront.
  // Create an application solely for parser needs (fortunately it is cheap).
  QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption info_option("info", "Show system environment information.");
  parser.addOption(info_option);

  QCommandLineOption scale_option(
      "scale",
      "Rely on system high-DPI scale via QT_ variables for 4K. If option is "
      "absent, will rely on internal scale mechanism (preferred).");
  parser.addOption(scale_option);

  QCommandLineOption font_option("font", "Main application font point size.");
  font_option.setValueName("pt");
  parser.addOption(font_option);

  QCommandLineOption style_option("style", "Main GUI style, see --info for full list available.");
  style_option.setValueName("name");
  parser.addOption(style_option);

  QCommandLineOption file_option(
      {"f", "file"},
      "Load sequencer procedure from a given file name; load multiple procedures if it is a folder",
      "filename");
  parser.addOption(file_option);

  parser.process(app);

  Options result;
  result.info = parser.isSet(info_option);
  result.scale = parser.isSet(scale_option);

  if (parser.isSet(font_option))
  {
    result.system_font_psize = parser.value(font_option).toInt();
  }

  if (parser.isSet(style_option))
  {
    result.style = parser.value(style_option);
  }

  if (parser.isSet(file_option))
  {
    result.file_name = parser.value(file_option);
  }

  return result;
}
}  // namespace sequencergui
