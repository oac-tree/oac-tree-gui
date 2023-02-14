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

#include "sequencergui/mainwindow/recent_project_settings.h"

#include <mvvm/utils/file_utils.h>

#include <QDir>
#include <QSettings>

namespace
{
const int kMaxRecentProjects = 10;

const QString kGroupKey = "welcomeview";
const QString kCurrentWorkdirKey = "currentworkdir";
const QString kRecentProjectsKey = "recentprojects";

QString GetWorkdirSettingName()
{
  return kGroupKey + "/" + kCurrentWorkdirKey;
}

QString GetRecentProjectsSettingName()
{
  return kGroupKey + "/" + kRecentProjectsKey;
}

}  // namespace

namespace sequencergui
{

RecentProjectSettings::RecentProjectSettings()
{
  ReadSettings();
}

RecentProjectSettings::~RecentProjectSettings()
{
  WriteSettings();
}

//! Returns current workdir.
QString RecentProjectSettings::GetCurrentWorkdir() const
{
  return m_current_workdir;
}

//! Updates current workdir value from user selection.
//! Workdir will be set as parent director of selected `dirname`.
void RecentProjectSettings::UpdateWorkdirFromSelection(const QString& dirname)
{
  if (!dirname.isEmpty())
  {
    auto parent_path = mvvm::utils::GetParentPath(dirname.toStdString());
    m_current_workdir = QString::fromStdString(parent_path);
  }
}

//! Returns list of recent projects, validates if projects still exists on disk.
QStringList RecentProjectSettings::GetRecentProjects()
{
  QStringList updated_list;
  for (const auto& file_name : m_recent_projects)
  {
    if (mvvm::utils::IsExists(file_name.toStdString()))
    {
      updated_list.append(file_name);
    }
  }
  m_recent_projects = updated_list;
  return m_recent_projects;
}

//! Adds directory to the list of recent projects.
void RecentProjectSettings::AddToRecentProjects(const QString& dirname)
{
  m_recent_projects.removeAll(dirname);
  m_recent_projects.prepend(dirname);
  while (m_recent_projects.size() > kMaxRecentProjects)
  {
    m_recent_projects.removeLast();
  }
}

void RecentProjectSettings::ClearRecentProjectsList()
{
  m_recent_projects.clear();
}

//! Write all settings to file.
void RecentProjectSettings::WriteSettings()
{
  QSettings settings;
  settings.setValue(GetWorkdirSettingName(), m_current_workdir);
  settings.setValue(GetRecentProjectsSettingName(), m_recent_projects);
}

//! Reads all settings from file.
void RecentProjectSettings::ReadSettings()
{
  QSettings settings;
  m_current_workdir = QDir::homePath();

  if (settings.contains(GetWorkdirSettingName()))
  {
    m_current_workdir = settings.value(GetWorkdirSettingName()).toString();
  }

  if (settings.contains(GetRecentProjectsSettingName()))
  {
    m_recent_projects = settings.value(GetRecentProjectsSettingName()).toStringList();
  }
}

}  // namespace sequencergui
