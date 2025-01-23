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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac-tree-gui/components/custom_meta_types.h>
#include <oac-tree-gui/core/version.h>
#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/mainwindow/command_line_options.h>
#include <oac-tree-gui/mainwindow/pvmonitor_main_window.h>
#include <sup/gui/mainwindow/main_window_helper.h>

#include <QApplication>
#include <QMessageBox>
#include <QMetaType>

int main(int argc, char** argv)
{
  auto version = QString::fromStdString(sequencergui::ProjectVersion());
  sup::gui::InitCoreApplication("sup-pvmonitor", version);

  auto options = sequencergui::ParseOptions(argc, argv);
  sup::gui::SetupHighDpiScaling(options.scale);

  QApplication app(argc, argv);

  sup::gui::SetupApplication(options.system_font_psize, options.style, options.info);

  sequencergui::RegisterCustomMetaTypes();
  Q_INIT_RESOURCE(oac_tree_icons);

  auto [success, message] = sequencergui::LoadPlugins();
  if (!success)
  {
    QMessageBox::warning(nullptr, "Failed to load plugins", QString::fromStdString(message));
  }

  sequencergui::PvMonitorMainWindow win;
  win.show();

  return app.exec();
}
