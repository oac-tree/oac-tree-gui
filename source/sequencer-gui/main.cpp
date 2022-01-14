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

#include "sequencergui/model/domainutils.h"

#include <QApplication>
#include <QLocale>
#include <QMetaType>

int main(int argc, char** argv)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  QApplication app(argc, argv);

  Q_INIT_RESOURCE(sequencericons);

#ifdef SEQUENCERGUI_CODAC
  sequencergui::DomainUtils::LoadPlugins();
#endif

  sequencergui::MainWindow win;

  win.show();

  return app.exec();
}
