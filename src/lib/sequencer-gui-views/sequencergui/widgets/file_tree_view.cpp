/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "file_tree_view.h"

#include <sequencergui/style/style_helper.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/widgets/action_menu.h>

#include <mvvm/project/recent_project_settings.h>
#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMenu>
#include <QSettings>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
//! Returns true if given file is a procedure file.
bool IsProcedureFile(const QFileInfo &info)
{
  return info.isFile() && info.completeSuffix().toLower() == QStringLiteral("xml");
}
}  // namespace

namespace sequencergui
{

FileTreeView::FileTreeView(QWidget *parent)
    : QWidget(parent)
    , m_file_system_model(new QFileSystemModel(this))
    , m_tree_view(new QTreeView)
    , m_path_label(new QLabel)
    , m_recent_dirs(std::make_unique<mvvm::RecentProjectSettings>("FileTreeView"))
    , m_bookmark_menu(std::make_unique<QMenu>())
{
  setWindowTitle("EXPLORER");
  setToolTip("File explorer");

  m_tree_view->setModel(m_file_system_model);
  m_tree_view->setColumnHidden(1, true);
  m_tree_view->setColumnHidden(2, true);
  m_tree_view->setColumnHidden(3, true);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_path_label);
  layout->addSpacing(5);
  layout->addWidget(m_tree_view);

  m_path_label->setWordWrap(true);
  connect(m_path_label, &QLabel::linkActivated, this, [this](auto link) { OnLabelClick(link); });

  connect(m_tree_view, &QTreeView::doubleClicked, this,
          [this](auto index) { OnTreeDoubleClick(index); });
  connect(m_tree_view, &QTreeView::clicked, this, [this](auto index) { OnTreeSingleClick(index); });

  SetCurrentDir(m_recent_dirs->GetCurrentWorkdir());

  sup::gui::utils::BeautifyTreeStyle(m_tree_view);

  SetupActions();
}

FileTreeView::~FileTreeView() = default;

void FileTreeView::SetCurrentDir(const QString &dirname)
{
  m_path_label->setText(mvvm::utils::CreatePathPresentation(dirname));
  m_file_system_model->setRootPath(dirname);
  const QModelIndex root_index = m_file_system_model->index(dirname);
  m_tree_view->setRootIndex(root_index);
  m_recent_dirs->SetCurrentWorkdir(dirname);
}

void FileTreeView::OnLabelClick(const QString &link)
{
  SetCurrentDir(link);
}

void FileTreeView::OnTreeDoubleClick(const QModelIndex &index)
{
  const QFileInfo info(m_file_system_model->filePath(index));

  if (info.isFile())
  {
    SetCurrentDir(info.dir().path());  // directory where clicked file is located
  }
  else
  {
    SetCurrentDir(info.filePath());  // directory which was clicked
  }

  OnImportFromFileRequest();
}

void FileTreeView::OnImportFromFileRequest()
{
  for (auto index : m_tree_view->selectionModel()->selectedIndexes())
  {
    if (index.column() == 0)
    {
      const QFileInfo info(m_file_system_model->filePath(index));
      if (IsProcedureFile(info))
      {
        emit ProcedureFileDoubleClicked(info.filePath());
      }
    }
  }
}

void FileTreeView::OnTreeSingleClick(const QModelIndex &index)
{
  const QFileInfo info(m_file_system_model->filePath(index));

  if (info.isFile() && info.completeSuffix().toLower() == QStringLiteral("xml"))
  {
    if (m_current_xml_file != info.filePath())
    {
      m_current_xml_file = info.filePath();
      emit ProcedureFileClicked(m_current_xml_file);
    }
  }
}

void FileTreeView::SetupActions()
{
  m_import_file_action = new QAction(this);
  m_import_file_action->setText("Import Selected");
  m_import_file_action->setToolTip(
      "Import procedure from currently selected XML file\n"
      "(alternatively, double-click on it)");
  m_import_file_action->setIcon(FindIcon("file-import-outline"));
  auto on_import_from_file = [this]() {};
  connect(m_import_file_action, &QAction::triggered, this, &FileTreeView::OnImportFromFileRequest);
  addAction(m_import_file_action);

  // Bookmark action
  m_bookmark_action = new sup::gui::ActionMenu(this);
  connect(m_bookmark_menu.get(), &QMenu::aboutToShow, this,
          &FileTreeView::OnAboutToShowBookmarkMenu);
  m_bookmark_action->setText("Bookmark");
  m_bookmark_action->setIcon(FindIcon("bookmark-outline"));
  m_bookmark_action->setMenu(m_bookmark_menu.get());
  m_bookmark_action->setToolTip("Manage bookmarks");
  addAction(m_bookmark_action);
}

void FileTreeView::OnAboutToShowBookmarkMenu()
{
  // We clear menu and populate it with entries: add bookmark, clear bookmarks and go to list of
  // bookmarks. This is done just before showing the menu to check if directories are still exist.
  // Class RecentProjectSettings will take care that directories exist, and there are not
  // duplications.

  m_bookmark_menu->clear();
  m_bookmark_menu->setToolTipsVisible(true);
  auto recent_projects = m_recent_dirs->GetRecentProjectList();

  // Action to add current dir to bookmarks.
  auto bookmark_action = m_bookmark_menu->addAction("Add bookmark");
  bookmark_action->setToolTip("Bookmark current directory");
  auto on_bookmark_action = [this]()
  { m_recent_dirs->AddToRecentProjectList(m_recent_dirs->GetCurrentWorkdir()); };
  connect(bookmark_action, &QAction::triggered, this, on_bookmark_action);

  // Action to clear all existing bookmarks.
  auto clear_action = m_bookmark_menu->addAction("Clear bookmarks");
  clear_action->setToolTip("Clear all bookmarks");
  auto on_clear_action = [this]() { m_recent_dirs->ClearRecentProjectsList(); };
  connect(clear_action, &QAction::triggered, this, on_clear_action);
  clear_action->setEnabled(!recent_projects.isEmpty());

  // Actions to go to bookmarks.
  m_bookmark_menu->addSeparator();
  for (const auto &project_dir : recent_projects)
  {
    auto trimmed_project_dir = mvvm::utils::WithTildeHomePath(project_dir);
    auto action = m_bookmark_menu->addAction(trimmed_project_dir);
    auto on_project_selected = [this, project_dir]() { SetCurrentDir(project_dir); };
    connect(action, &QAction::triggered, this, on_project_selected);
  }
}

}  // namespace sequencergui
