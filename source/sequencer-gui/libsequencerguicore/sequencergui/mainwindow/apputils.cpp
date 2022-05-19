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

#include "sequencergui/mainwindow/apputils.h"

#include "mvvm/widgets/widgetutils.h"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QSize>
#include <QString>
#include <sstream>

namespace
{
QString GetEnvironmentInfo()
{
  QString result;
  result += "QT_AUTO_SCREEN_SCALE_FACTOR  : " + qEnvironmentVariable("QT_AUTO_SCREEN_SCALE_FACTOR");
  result += "\n";
  result += "QT_SCALE_FACTOR              : " + qEnvironmentVariable("QT_SCALE_FACTOR");
  result += "\n";
  result += "QT_SCREEN_SCALE_FACTORS      : " + qEnvironmentVariable("QT_SCREEN_SCALE_FACTORS");
  result += "\n";
  return result;
}

QString GetFontInfo()
{
  QString result;
  auto size = ::mvvm::utils::SizeOfLetterM();
  result +=
      "Size of letter M             : " + QString("(%1, %2)").arg(size.width()).arg(size.height());
  result += "\n";

  auto font = QGuiApplication::font();
  result += "Default font point size      : "
            + QString("%1 (%2F)").arg(font.pointSize()).arg(font.pointSizeF());
  result += "\n";

  return result;
}

QString GetScreenInfo()
{
  QString result;
  auto screen = QGuiApplication::primaryScreen();
  auto size = screen->physicalSize();
  result +=
      "Screen physical size         : " + QString("(%1, %2)").arg(size.height()).arg(size.width());
  result += "\n";
  auto geometry = screen->geometry();
  result += "Screen geometry              : "
            + QString("(%1, %2)").arg(geometry.height()).arg(geometry.width());
  result += "\n";

  result += "Device pixel ratio           : " + QString("%1").arg(screen->devicePixelRatio());
  result += "\n";

  result += "Logical dots per inch        : "
            + QString("%1 (%2, %3)")
                  .arg(screen->logicalDotsPerInch())
                  .arg(screen->logicalDotsPerInchX())
                  .arg(screen->logicalDotsPerInchY());
  result += "\n";

  result += "Physical dots per inch       : "
            + QString("%1 (%2, %3)")
                  .arg(screen->physicalDotsPerInch())
                  .arg(screen->physicalDotsPerInchX())
                  .arg(screen->physicalDotsPerInchY());
  result += "\n";

  return result;
}

//! Returns true if there is an attempt to scale via environment variable.
bool IsAttemptToScaleViaEnvironment()
{
  return qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
         || qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
         || qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS");
}

void ResetHighDpiEnvironment()
{
  qunsetenv("QT_AUTO_SCREEN_SCALE_FACTOR");
  qunsetenv("QT_SCALE_FACTOR");
  qunsetenv("QT_SCREEN_SCALE_FACTORS");
}

}  // namespace

namespace sequencergui
{

std::string GetDesktopInfo()
{
  const int line_length(80);
  QString result;

  result += QString(line_length, '-');
  result += "\n";
  result += GetEnvironmentInfo();
  result += GetFontInfo();
  result += GetScreenInfo();

  result += QString(line_length, '-');
  result += "\n";

  return result.toStdString();
}

Options ParseOptions(int argc, char **argv)
{
  // Parser requires an application to be created upfront, fortunately it is cheap.
  // Create an application solely for parser needs.
  QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption info_option("info", "Show system environment information");
  parser.addOption(info_option);

  QCommandLineOption scale_option("scale",
                                  "Rely on system scale via QT_ variables for 4K. If option is "
                                  "absent, will rely on internal scale mechanism.");
  parser.addOption(scale_option);

  QCommandLineOption font_option("font", "Main application font point size");
  font_option.setValueName("10");
  parser.addOption(font_option);

  parser.process(app);

  Options result;
  result.info = parser.isSet(info_option);
  result.scale = parser.isSet(scale_option);

  if (parser.isSet(font_option))
  {
    result.system_font_psize = parser.value(font_option).toInt();
  }

  return result;
}

void SetupHighDpiScaling(bool use_system_scale)
{
  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  if (use_system_scale)
  {
    if (IsAttemptToScaleViaEnvironment())
    {
      // Nothing to do if scaling is steared via environment variables
      qDebug() << "QT_ defined, no scale";
      return;
    }
    qDebug() << "Enable High Dpi";
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
  }
  else
  {
    qDebug() << "Own scaling, resetting environment";
    // our GUI is perfect and knows how to scale by itself
    ResetHighDpiEnvironment();
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling, true);
  }
}

}  // namespace sequencergui
