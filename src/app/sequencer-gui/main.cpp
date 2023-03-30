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

#include <mvvm/widgets/app_utils.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/mainwindow/command_line_options.h>
#include <sequencergui/mainwindow/sequencer_main_window.h>
#include <sequencergui/widgets/custom_meta_types.h>

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
  QCoreApplication::setOrganizationName("coa");

  auto options = sequencergui::ParseOptions(argc, argv);

  mvvm::utils::SetupHighDpiScaling(options.scale);

  QApplication app(argc, argv);

  sequencergui::RegisterCustomMetaTypes();

  mvvm::utils::SetApplicationFontSize(options.system_font_psize);

  if (options.info)
  {
    std::cout << mvvm::utils::GetDesktopInfo();
  }

  Q_INIT_RESOURCE(sequencericons);

  sequencergui::LoadPlugins();

  sequencergui::SequencerMainWindow win;

  win.show();

  return app.exec();
}
