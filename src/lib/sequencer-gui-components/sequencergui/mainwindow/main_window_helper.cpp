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

#include "main_window_helper.h"

#include <mvvm/utils/file_utils.h>

#include <QMessageBox>
#include <QPushButton>
#include <filesystem>

namespace
{
const std::string kProcedureExtension(".xml");
}

namespace sequencergui
{

std::vector<std::string> GetProcedureFiles(const std::string &path_name)
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

}  // namespace sequencergui
