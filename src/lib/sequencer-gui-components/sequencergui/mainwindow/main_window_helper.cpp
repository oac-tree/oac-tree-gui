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

#include <QFontDialog>
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

bool ShouldResetSettingsAndRestart()
{
  const QString question_text =
      "All persistent application settings (i.e. window sizes, position of splitters, etc) "
      " will be reset to their default values. Restart is required.";

  QMessageBox msgBox;
  msgBox.setText(question_text);
  msgBox.setInformativeText("Do you want to reset settings and restart application?\n");

  auto yes_button = msgBox.addButton("Yes, please restart", QMessageBox::YesRole);
  msgBox.addButton("Cancel", QMessageBox::NoRole);

  msgBox.exec();
  return msgBox.clickedButton() == yes_button;
}

void SummonChangeSystemFontDialog()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, QApplication::font(), nullptr);
  if (ok)
  {
    // the user clicked OK and font is set to the font the user selected
    SaveAppFontInSettings(font);

    QApplication::setFont(font);
    // the problem is static variable in mvvm::FindSizeOfLetterM, it's not enough to restart the
    // window, the whole application should be restarted

    QMessageBox::information(nullptr, "Restart is required",
                             "Please restart application to fully apply changes");
  }
}

}  // namespace sequencergui
