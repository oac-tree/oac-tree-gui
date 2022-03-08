/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/mainwindow/projecthandler.h"

#include "sequencergui/mainwindow/recentprojectsettings.h"
#include "sequencergui/mainwindow/userinteractor.h"

#include "mvvm/factories/projectmanagerfactory.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/project/project_types.h"
#include "mvvm/widgets/widgetutils.h"

#include <QMainWindow>

namespace sequencergui
{

ProjectHandler::ProjectHandler(mvvm::ApplicationModel* model, QWidget* parent)
    : QObject(parent)
    , m_recentProjectSettings(std::make_unique<RecentProjectSettings>())
    , m_userInteractor(std::make_unique<UserInteractor>(m_recentProjectSettings.get(), parent))
    , m_model(model)
{
  initProjectManager();
  updateRecentProjectNames();
}

ProjectHandler::~ProjectHandler() = default;

//! Update names (name of the current project, recent project name list, notifies the world).

void ProjectHandler::updateNames()
{
  updateCurrentProjectName();
  updateRecentProjectNames();
}

//! Returns 'true' if current project can be closed.
//! Internally will perform check for unsaved data, and proceed via save/discard/cancel dialog.

bool ProjectHandler::canCloseProject() const
{
  return m_projectManager->CloseCurrentProject();
}

void ProjectHandler::onCreateNewProject()
{
  if (m_projectManager->CreateNewProject({}))
    updateNames();
}

void ProjectHandler::onOpenExistingProject(const QString& dirname)
{
  if (m_projectManager->OpenExistingProject(dirname.toStdString()))
    updateNames();
}

void ProjectHandler::onSaveCurrentProject()
{
  if (m_projectManager->SaveCurrentProject())
    updateNames();
}

void ProjectHandler::onSaveProjectAs()
{
  if (m_projectManager->SaveProjectAs({}))
    updateNames();
}

void ProjectHandler::clearRecentProjectsList()
{
  m_recentProjectSettings->ClearRecentProjectsList();
  updateNames();
}

void ProjectHandler::initProjectManager()
{
  auto modified_callback = [this]() { updateCurrentProjectName(); };
  auto models_callback = [this]() { return std::vector<mvvm::ApplicationModel*>({m_model}); };
  mvvm::ProjectContext project_context{modified_callback, models_callback};

  auto select_dir_callback = [this]() { return m_userInteractor->OnSelectDirRequest(); };
  auto create_dir_callback = [this]() { return m_userInteractor->OnCreateDirRequest(); };
  auto answer_callback = [this]() { return m_userInteractor->OnSaveChangesRequest(); };
  mvvm::UserInteractionContext user_context{select_dir_callback, create_dir_callback,
                                            answer_callback};

  m_projectManager = CreateProjectManager(project_context, user_context);
}

//! Updates the name of the current project on main window, notifies the world.

void ProjectHandler::updateCurrentProjectName()
{
  const auto current_project_dir = QString::fromStdString(m_projectManager->CurrentProjectDir());
  const auto is_modified = m_projectManager->IsModified();

  // set main window title
  auto title = mvvm::utils::ProjectWindowTitle(current_project_dir, is_modified);
  if (auto main_window = mvvm::utils::FindMainWindow(); main_window)
    main_window->setWindowTitle(title);

  currentProjectModified(current_project_dir, is_modified);
}

//! Update recent project list in settings, notifies the world.

void ProjectHandler::updateRecentProjectNames()
{
  m_recentProjectSettings->AddToRecentProjects(
      QString::fromStdString(m_projectManager->CurrentProjectDir()));
  recentProjectsListModified(m_recentProjectSettings->GetRecentProjects());
}

}  // namespace sequencergui
