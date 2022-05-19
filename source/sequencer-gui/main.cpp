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

#include "sequencergui/mainwindow/mainwindow.h"
#include "sequencergui/mainwindow/apputils.h"
#include "sequencergui/model/domainutils.h"

#include <QApplication>
#include <QLocale>
#include <QMetaType>
#include <QStyleFactory>
#include <iostream>

int main(int argc, char** argv)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

  QCoreApplication::setApplicationName("sequencer-gui");
  QCoreApplication::setApplicationVersion("0.1");
  QCoreApplication::setOrganizationName("sequencer");

  auto options = sequencergui::ParseOptions(argc, argv);

  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);

  QApplication app(argc, argv);

  if (options.info)
  {
    std::cout << sequencergui::GetDesktopInfo();
  }

  QApplication::setStyle(QStyleFactory::create("Breeze")); // same style on all machines

//  auto font = QApplication::font();
//  font.setPointSize(font.pointSize()*1.5);
//  QApplication::setFont(font);

  Q_INIT_RESOURCE(sequencericons);

#ifdef SEQUENCERGUI_CODAC
  sequencergui::DomainUtils::LoadPlugins();
#endif

  sequencergui::MainWindow win;

  win.show();

  return app.exec();
}
