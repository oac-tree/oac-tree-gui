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

#include <sequencergui/components/custom_meta_types.h>
#include <sequencergui/core/version.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/mainwindow/command_line_options.h>
#include <sequencergui/mainwindow/main_window_helper.h>
#include <sequencergui/mainwindow/sequencer_main_window.h>
#include <sequencergui/mainwindow/splash_screen.h>
#include <sup/gui/widgets/application_helper.h>

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QMetaType>

int main(int argc, char** argv)
{
  auto version = QString::fromStdString(sequencergui::ProjectVersion());
  sup::gui::InitCoreApplication("sequencer-gui", version);

  auto options = sequencergui::ParseOptions(argc, argv);
  sup::gui::SetupHighDpiScaling(options.scale);

  QApplication app(argc, argv);

  sup::gui::SetWindowStyle(options.style, options.system_font_psize, options.info);

  sequencergui::RegisterCustomMetaTypes();
  Q_INIT_RESOURCE(sequencericons);

  auto [success, message] = sequencergui::LoadPlugins();
  if (!success)
  {
    QMessageBox::warning(nullptr, "Failed to load plugins", QString::fromStdString(message));
  }

  std::unique_ptr<sequencergui::SplashScreen> splash;

  if (!options.file_name.isEmpty())
  {
    splash.reset(new sequencergui::SplashScreen);
    splash->Start(/*show_during*/ 2000);
  }

  sequencergui::SequencerMainWindow win;
  for (auto file_name : sequencergui::GetProcedureFiles(options.file_name.toStdString()))
  {
    if (!win.ImportProcedure(QString::fromStdString(file_name)))
    {
      qInfo() << "Failed to load procedure from file" << QString::fromStdString(file_name);
    }
  }

  win.show();

  if (splash)
  {
    splash->finish(&win);
  }

  return app.exec();
}
