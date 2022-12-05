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

#include "main_window.h"
#include <mvvm/widgets/app_utils.h>
#include <sequencergui/domain/domain_utils.h>

#include <QApplication>
#include <QLocale>

int main(int argc, char** argv)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

  mvvm::utils::SetupHighDpiScaling(false);

  QApplication app(argc, argv);

  Q_INIT_RESOURCE(sequencericons);

  sequencergui::DomainUtils::LoadPlugins();

  suppvmonitor::MainWindow win;

  win.show();

  return app.exec();
}
