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

#include <sup/gui/mainwindow/main_window_helper.h>

#include <QApplication>
#include <QMessageBox>
#include <QMetaType>

#include <oac_tree_gui/components/load_resources.h>
#include <oac_tree_gui/core/version.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/mainwindow/command_line_options.h>
#include <oac_tree_gui/mainwindow/pvmonitor_main_window.h>

int main(int argc, char** argv)
{
  oac_tree_gui::RegisterCustomMetaTypes();

  auto version = QString::fromStdString(oac_tree_gui::ProjectVersion());
  sup::gui::InitCoreApplication("sup-pvmonitor", version);

  auto options = oac_tree_gui::ParseOptions(argc, argv);
  sup::gui::SetupHighDpiScaling(options.scale);

  const QApplication app(argc, argv);

  sup::gui::SetupApplication(options.system_font_psize, options.style, options.info);

  oac_tree_gui::LoadOacTreePlugins();
  oac_tree_gui::LoadOacTreeItems();

  oac_tree_gui::PvMonitorMainWindow win;
  win.show();

  return app.exec();
}
