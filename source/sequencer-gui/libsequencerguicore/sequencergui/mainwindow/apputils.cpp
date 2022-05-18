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

#include <QApplication>
#include <QDesktopWidget>
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
  return result;
}

QString GetDpiInfo()
{
  QString result;
  int x = QApplication::desktop()->physicalDpiX();
  int y = QApplication::desktop()->physicalDpiY();
  result += "Physical Dpi                 : " + QString("(%1, %2)").arg(x).arg(y);
  result += "\n";
  return result;
}
}  // namespace

namespace sequencergui
{

std::string GetDesktopInfo()
{
  QString result;

  result += QString(80, '-');
  result += "\n";
  result += GetEnvironmentInfo();
  result += GetFontInfo();
  result += GetDpiInfo();

  result += QString(80, '-');
  result += "\n";

  return result.toStdString();
}

}  // namespace sequencergui
