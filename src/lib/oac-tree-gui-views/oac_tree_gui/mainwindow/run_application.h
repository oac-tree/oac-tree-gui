/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_MAINWINDOW_RUN_APPLICATION_H_
#define OAC_TREE_GUI_MAINWINDOW_RUN_APPLICATION_H_

#include <oac_tree_gui/components/load_resources.h>
#include <oac_tree_gui/mainwindow/command_line_options.h>
#include <oac_tree_gui/mainwindow/main_window_helper.h>
#include <oac_tree_gui/mainwindow/sequencer_main_window_context.h>
#include <oac_tree_gui/mainwindow/splash_screen.h>

#include <sup/gui/app/main_window_types.h>
#include <sup/gui/mainwindow/main_window_helper.h>

#include <mvvm/widgets/app_utils.h>

#include <QApplication>
#include <QSettings>

namespace oac_tree_gui
{

/**
 * @brief Create QApplication and main window.
 * @tparam MainWindowT The type of main window to create.
 */
template <typename MainWindowT>
int RunApplication(int argc, char** argv)
{
  auto options = oac_tree_gui::ParseOptions(argc, argv);
  mvvm::utils::SetupHighDpiScaling(options.scale);

  oac_tree_gui::LoadOacTreeItems();

  QApplication app(argc, argv);

  const auto default_font = app.font();

  sup::gui::SetupApplication(options.system_font_psize, options.style, options.info);

  std::unique_ptr<oac_tree_gui::SplashScreen> splash;

  // splash screen will be shown only if there are procedures to load
  if (!options.file_name.isEmpty())
  {
    splash = std::make_unique<oac_tree_gui::SplashScreen>();
    splash->Start(/*show_during*/ 1000);
  }

  int exit_code{0};
  do
  {
    if (exit_code == sup::gui::CleanSettingsAndRestart)
    {
      QSettings settings;
      settings.clear();
      QApplication::setFont(default_font);
    }

    SequencerMainWindowContext context;
    context.LoadPlugins();
    MainWindowT win(context);

    if (options.window_size.has_value())
    {
      win.resize(options.window_size.value());
    }
    win.show();
    auto on_import = [&win](auto file_name) { return win.ImportProcedure(file_name); };
    ImportProcedures(options.file_name, on_import);

    if (splash)
    {
      splash->finish(&win);
      splash.reset();
    }

    exit_code = app.exec();
  } while (exit_code != sup::gui::NormalExit);

  return exit_code;
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_RUN_APPLICATION_H_
