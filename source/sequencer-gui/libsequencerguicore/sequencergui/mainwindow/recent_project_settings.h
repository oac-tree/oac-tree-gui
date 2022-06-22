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

#ifndef SEQUENCERGUI_MAINWINDOW_RECENT_PROJECT_SETTINGS_H_
#define SEQUENCERGUI_MAINWINDOW_RECENT_PROJECT_SETTINGS_H_

#include <QStringList>

namespace sequencergui
{

//! Collection of settings to save the last directory selected by the user, and list of recent
//! projects. Relies on QSettings machinery.

class RecentProjectSettings
{
public:
  RecentProjectSettings();
  ~RecentProjectSettings();

  QString GetCurrentWorkdir() const;

  void UpdateWorkdirFromSelection(const QString& dirname);

  QStringList GetRecentProjects();

  void AddToRecentProjects(const QString& dirname);

  void ClearRecentProjectsList();

private:
  void WriteSettings();
  void ReadSettings();

  QString m_current_workdir;
  QStringList m_recent_projects;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_RECENT_PROJECT_SETTINGS_H_
