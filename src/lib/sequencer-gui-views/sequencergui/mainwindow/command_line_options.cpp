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

#include "command_line_options.h"

#include <sup/gui/app/main_window_helper.h>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>

namespace sequencergui
{

namespace
{

const QString kInfoOption = "info";
const QString kScaleOption = "scale";
const QString kFontOption = "font";
const QString kStyleOption = "style";
const QString kFileOption = "file";
const QString kWindowSizeOption = "size";

/**
 * @brief Populates parser with application command line options.
 */
void PopulateOptions(QCommandLineParser& parser)
{
  parser.addHelpOption();
  parser.addVersionOption();

  const QCommandLineOption info_option(kInfoOption, "Show system environment information.");
  parser.addOption(info_option);

  const QCommandLineOption scale_option(
      "scale",
      "Rely on system high-DPI scale via QT_ variables for 4K. If option is "
      "absent, will rely on internal scale mechanism (preferred).");
  parser.addOption(scale_option);

  QCommandLineOption font_option(kFontOption, "Main application font point size.");
  font_option.setValueName("pt");
  parser.addOption(font_option);

  QCommandLineOption style_option(kStyleOption,
                                  "Main GUI style, see --info for full list available.");
  style_option.setValueName("name");
  parser.addOption(style_option);

  const QCommandLineOption file_option(
      {kFileOption, "f"},
      "Load sequencer procedure from a given file name; load multiple procedures if it is a folder",
      "filename");
  parser.addOption(file_option);

  QCommandLineOption window_size_option(kWindowSizeOption, "Initial window size");
  window_size_option.setValueName("1024x768");
  parser.addOption(window_size_option);
}

/**
 * @brief Parses command line options and returns structure with options.
 */
Options ParseOptions(QCommandLineParser& parser)
{
  Options result;
  result.info = parser.isSet(kInfoOption);
  result.scale = parser.isSet(kScaleOption);

  if (parser.isSet(kFontOption))
  {
    result.system_font_psize = parser.value(kFontOption).toInt();
  }

  if (parser.isSet(kStyleOption))
  {
    result.style = parser.value(kStyleOption);
  }

  if (parser.isSet(kFileOption))
  {
    result.file_name = parser.value(kFileOption);
  }

  if (parser.isSet(kWindowSizeOption))
  {
    if (auto optional_size = sup::gui::ParseSizeString(parser.value(kWindowSizeOption));
        optional_size.has_value())
    {
      result.window_size = optional_size;
    }
    else
    {
      qInfo() << "Can't parse windows size option" << parser.value(kWindowSizeOption);
      parser.showHelp(1);
    }
  }
  return result;
}

}  // namespace

Options ParseOptions(int argc, char** argv)
{
  // Parser requires an application to be created upfront.
  // Create an application solely for parser needs (fortunately it is cheap).
  QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  PopulateOptions(parser);
  parser.process(app);
  return ParseOptions(parser);
}
}  // namespace sequencergui
