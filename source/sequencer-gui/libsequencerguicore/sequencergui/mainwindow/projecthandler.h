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

#ifndef SEQUENCERGUI_MAINWINDOW_PROJECTHANDLER_H
#define SEQUENCERGUI_MAINWINDOW_PROJECTHANDLER_H

#include <QObject>
#include <memory>
#include <vector>

class QWidget;

namespace mvvm
{
class ProjectManagerInterface;
class ApplicationModel;
}  // namespace mvvm

namespace sequencergui
{

class RecentProjectSettings;
class UserInteractor;
class ApplicationModels;
class RecentProjectWidget;

//! Main class to coordinate all activity on user's request to create new project,
//! open existing one, or choose one of recent projects on disk.

class ProjectHandler : public QObject
{
  Q_OBJECT

public:
  explicit ProjectHandler(mvvm::ApplicationModel* model, QWidget* parent);
  ~ProjectHandler() override;

  bool canCloseProject() const;
  void updateNames();
  void onCreateNewProject();
  void onOpenExistingProject(const QString& dirname = {});
  void onSaveCurrentProject();
  void onSaveProjectAs();

  void clearRecentProjectsList();

signals:
  void currentProjectModified(const QString& project_dir, bool is_modified);
  void recentProjectsListModified(const QStringList& projects);

private:
  void initProjectManager();
  void updateCurrentProjectName();
  void updateRecentProjectNames();

  std::unique_ptr<RecentProjectSettings> m_recentProjectSettings;
  std::unique_ptr<UserInteractor> m_userInteractor;
  std::unique_ptr<mvvm::ProjectManagerInterface> m_projectManager;
  mvvm::ApplicationModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_PROJECTHANDLER_H
