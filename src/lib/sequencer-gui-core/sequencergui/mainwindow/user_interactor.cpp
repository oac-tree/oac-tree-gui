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

#include "user_interactor.h"

#include "recent_project_settings.h"

#include <mvvm/project/project_types.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/utils/file_utils.h>

#include <QFileDialog>
#include <QMessageBox>
#include <map>

namespace
{
//! Map of standard Qt answers to what ProjectManager expects.
std::map<QMessageBox::StandardButton, mvvm::SaveChangesAnswer> answer_map()
{
  std::map<QMessageBox::StandardButton, mvvm::SaveChangesAnswer> result = {
      {QMessageBox::Save, mvvm::SaveChangesAnswer::kSave},
      {QMessageBox::Discard, mvvm::SaveChangesAnswer::kDiscard},
      {QMessageBox::Cancel, mvvm::SaveChangesAnswer::kCancel}};
  return result;
}
}  // namespace

namespace sequencergui
{

UserInteractor::UserInteractor(RecentProjectSettings* settings, QWidget* parent)
    : m_settings(settings), m_parent(parent)
{
}

//! Returns directory on disk selected by the user via QFileDialog.
//! Checks if selected directory can be the project directory.

std::string UserInteractor::OnSelectDirRequest()
{
  auto dirname = SummonSelectDialog();

  if (dirname.empty())  // no valid selection
  {
    return {};
  }

  if (!mvvm::ProjectUtils::IsPossibleProjectDir(dirname))
  {
    QMessageBox msgBox;
    msgBox.setText("Selected directory doesn't look like a project directory, choose another one");
    msgBox.exec();
    return {};
  }

  return dirname;
}

//! Returns new directory on disk created by the user via QFileDialog.

std::string UserInteractor::OnCreateDirRequest()

{
  auto dirname = SummonSelectDialog();

  if (dirname.empty())  // no valid selection
  {
    return {};
  }

  if (!mvvm::utils::IsEmpty(dirname))
  {
    QMessageBox msgBox;
    msgBox.setText("The selected directory is not empty, choose another one.");
    msgBox.exec();
    return {};
  }

  return dirname;
}

//! Returns save/cancel/discard changes choice provided by the user.

mvvm::SaveChangesAnswer UserInteractor::OnSaveChangesRequest()
{
  static auto translate = answer_map();

  QMessageBox msgBox;
  msgBox.setText("The project has been modified.");
  msgBox.setInformativeText("Do you want to save your changes?");
  msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);
  auto ret = static_cast<QMessageBox::StandardButton>(msgBox.exec());
  return translate[ret];
}

//! Summon dialog to select directory on disk. If selection is not empty,
//! save parent directory for later re-use.

std::string UserInteractor::SummonSelectDialog() const
{
  QString dirname = QFileDialog::getExistingDirectory(
      m_parent, "Select directory", m_settings->GetCurrentWorkdir(),
      QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

  if (!dirname.isEmpty())
  {
    m_settings->UpdateWorkdirFromSelection(dirname);
  }

  return dirname.toStdString();
}

}  // namespace sequencergui
