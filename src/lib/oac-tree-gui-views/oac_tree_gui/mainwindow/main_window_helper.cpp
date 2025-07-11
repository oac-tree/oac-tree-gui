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

#include "main_window_helper.h"

#include <oac_tree_gui/domain/domain_helper.h>

#include <sup/gui/widgets/settings_callbacks.h>

#include <mvvm/utils/file_utils.h>

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <filesystem>

namespace
{
const std::string kProcedureExtension(".xml");
}  // namespace

namespace oac_tree_gui
{

bool ShouldStopRunningJobs()
{
  QMessageBox msgBox;
  msgBox.setText("Some procedures are in a running state.");
  msgBox.setInformativeText("Do you want to stop all running jobs?\n");

  auto yes_button = msgBox.addButton("Yes, stop jobs and quit", QMessageBox::YesRole);
  msgBox.addButton("Cancel", QMessageBox::NoRole);

  msgBox.exec();
  return msgBox.clickedButton() == yes_button;
}

std::vector<std::string> GetProcedureFiles(const std::string& path_name)
{
  std::vector<std::string> result;

  const std::filesystem::path path(path_name);

  if (std::filesystem::is_directory(path_name))
  {
    return mvvm::utils::FindFiles(path, kProcedureExtension);
  }

  if (path.extension() == kProcedureExtension)
  {
    result.push_back(path_name);
  }

  return result;
}

void ImportProcedures(const QString& file_name, const std::function<bool(const QString&)>& func)
{
  for (const auto& name : GetProcedureFiles(file_name.toStdString()))
  {
    if (func(QString::fromStdString(name)))
    {
      qInfo() << "Import OK:" << QString::fromStdString(name);
    }
    else
    {
      qInfo() << "Failed to load procedure from file" << QString::fromStdString(name);
    }
  }
}

QString GetOpenSequencerProcedureName(const QString& key)
{
  QString working_dir{};
  if (!key.isEmpty())
  {
    const auto read_func = sup::gui::GetSettingsReadFunc();
    working_dir = read_func(key).toString();
  }

  const QString result =
      QFileDialog::getOpenFileName(nullptr, "Open file", working_dir, "Files (*.xml *.XML)");

  if (!result.isEmpty() && !key.isEmpty())
  {
    const auto write_func = sup::gui::GetSettingsWriteFunc();
    const auto parent_path = mvvm::utils::GetParentPath(result.toStdString());
    write_func(key, QString::fromStdString(parent_path));
  }

  return result;
}

}  // namespace oac_tree_gui
