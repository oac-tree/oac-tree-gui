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

#ifndef SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
#define SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_

//! @file
//! Collection of helper methods common for all sequencer main windows.

#include <sequencergui/mainwindow/command_line_options.h>
#include <sequencergui/mainwindow/splash_screen.h>
#include <sup/gui/app/application_helper.h>
#include <sup/gui/app/main_window_types.h>

#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QSettings>
#include <functional>
#include <string>
#include <vector>

namespace sequencergui
{

/**
 * @brief Loads plugins and provide warning dialog if something went wrong.
 */
void LoadMainPlugins();

/**
 * @brief Create QApplication and main window.
 * @tparam MainWindowT The type of main window to create.
 */

template <typename MainWindowT>
int RunApplication(int argc, char** argv)
{
  auto options = sequencergui::ParseOptions(argc, argv);
  sup::gui::SetupHighDpiScaling(options.scale);

  QApplication app(argc, argv);

  const auto default_font = app.font();

  LoadMainPlugins();

  sup::gui::SetupApplication(options.system_font_psize, options.style, options.info);

  std::unique_ptr<sequencergui::SplashScreen> splash;

  // splash screen will be shown only if there are procedures to load
  if (!options.file_name.isEmpty())
  {
    splash.reset(new sequencergui::SplashScreen);
    splash->Start(/*show_during*/ 1000);
  }

  int exit_code{0};
  std::unique_ptr<MainWindowT> win;
  do
  {
    if (exit_code == sup::gui::CleanSettingsAndRestart)
    {
      QSettings settings;
      settings.clear();
      mvvm::utils::SetApplicationFont(default_font);
    }

    win = std::make_unique<MainWindowT>();
    win->show();
    auto on_import = [&win](auto file_name) { return win->ImportProcedure(file_name); };
    ImportProcedures(options.file_name, on_import);

    if (splash)
    {
      splash->finish(win.get());
      splash.reset();
    }

    exit_code = app.exec();
  } while (exit_code != sup::gui::NormalExit);

  return exit_code;
}

/**
 * @brief Opens a message box with the question if running jobs should be stopped.
 */
bool ShouldStopRunningJobs();

/**
 * @brief Returns vector of names representing sequencer procedures located in a given folder.
 *
 * @param path The name of the folder, or the file name itself.
 *
 * @details If given path is the procedure itself, will return a vector with this name as a single
 * element.
 */

std::vector<std::string> GetProcedureFiles(const std::string& path_name);

/**
 * @brief Loads procedures, print warnings if something went wrong.
 *
 * @param file_name The name of the file or directory to load procedures.
 * @param func A function to call on every procedure found.
 */
void ImportProcedures(const QString& file_name,
                      const std::function<bool(const QString& name)>& func);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
