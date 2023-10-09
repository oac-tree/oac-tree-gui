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

#ifndef SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
#define SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_

//! Collection of helper methods common for all main windows.

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/mainwindow/command_line_options.h>
#include <sequencergui/mainwindow/splash_screen.h>
#include <sup/gui/widgets/application_helper.h>

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QMetaType>
#include <string>
#include <vector>

namespace sequencergui
{

/**
 * @brief Returns vector of names representing sequencer procedures located in a given folder.
 *
 * @param path The name of the folder, or the file name itself.
 *
 * @details If given path is the procedure itself, will return a vector with this name as a single
 * element.
 */
std::vector<std::string> GetProcedureFiles(const std::string& path_name);

template <typename T>
int RunApplication(int argc, char** argv)
{
  auto options = sequencergui::ParseOptions(argc, argv);
  sup::gui::SetupHighDpiScaling(options.scale);

  QApplication app(argc, argv);

  sup::gui::SetWindowStyle(options.style, options.system_font_psize, options.info);

  auto [success, message] = sequencergui::LoadPlugins();
  if (!success)
  {
    QMessageBox::warning(nullptr, "Failed to load plugins", QString::fromStdString(message));
  }

  std::unique_ptr<sequencergui::SplashScreen> splash;

  // splash screen will be shown only if there are procedures to load
  if (!options.file_name.isEmpty())
  {
    splash.reset(new sequencergui::SplashScreen);
    splash->Start(/*show_during*/ 1000);
  }

  T win;
  for (auto file_name : sequencergui::GetProcedureFiles(options.file_name.toStdString()))
  {
    if (win.ImportProcedure(QString::fromStdString(file_name)))
    {
      qInfo() << "Import OK:" << QString::fromStdString(file_name);
    }
    else
    {
      qInfo() << "Failed to load procedure from file" << QString::fromStdString(file_name);
    }
  }

  win.show();

  if (splash)
  {
    splash->finish(&win);
    splash.reset();
  }

  return app.exec();
}

/**
 * @brief Opens a message box with the question if running jobs should be stopped.
 */
bool ShouldStopRunningJobs();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
