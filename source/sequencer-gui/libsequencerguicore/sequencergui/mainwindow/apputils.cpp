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

}  // namespace sequencergui
