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

#include <oac_tree_gui/components/custom_meta_types.h>
#include <oac_tree_gui/core/version.h>
#include <oac_tree_gui/mainwindow/main_window_helper.h>
#include <oac_tree_gui/mainwindow/operation_main_window.h>

int main(int argc, char** argv)
{
  oac_tree_gui::RegisterCustomMetaTypes(); // before InitCoreApplication

  auto version = QString::fromStdString(oac_tree_gui::ProjectVersion());
  sup::gui::InitCoreApplication("oac-tree-operation", version);

  return oac_tree_gui::RunApplication<oac_tree_gui::OperationMainWindow>(argc, argv);
}
