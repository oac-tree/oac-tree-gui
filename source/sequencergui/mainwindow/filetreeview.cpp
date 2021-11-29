/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/mainwindow/filetreeview.h"

#include "mvvm/widgets/widgetutils.h"

#include <QDebug>
#include <QFileSystemModel>
#include <QLabel>
#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kGroupKey = "filetreeview";
const QString kCurrentWorkdirKey = "currentworkdir";

const QString GetWorkdirSettingName()
{
  return kGroupKey + "/" + kCurrentWorkdirKey;
}

//! Returns true if given file is a procedure file.
bool IsProcedureFile(const QFileInfo &info)
{
  return info.isFile() && info.completeSuffix().toLower() == QStringLiteral("xml");
}
}  // namespace

namespace sequi
{
FileTreeView::FileTreeView(QWidget *parent)
    : QWidget(parent)
    , m_file_system_model(new QFileSystemModel(this))
    , m_tree_view(new QTreeView)
    , m_path_label(new QLabel)
{
  readSettings();

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
  connect(m_path_label, &QLabel::linkActivated, [this](auto link) { onLabelClick(link); });

  connect(m_tree_view, &QTreeView::doubleClicked, [this](auto index) { onTreeDoubleClick(index); });
  connect(m_tree_view, &QTreeView::clicked, [this](auto index) { onTreeSingleClick(index); });

  SetCurrentDir(m_current_workdir);
}

FileTreeView::~FileTreeView()
{
  writeSettings();
}

//! Sets the directory to be shown in breadcrumb, and in a file tree view.

void FileTreeView::SetCurrentDir(const QString &dirname)
{
  m_path_label->setText(ModelView::Utils::CreatePathPresentation(dirname));
  m_file_system_model->setRootPath(dirname);
  const QModelIndex root_index = m_file_system_model->index(dirname);
  m_tree_view->setRootIndex(root_index);
  m_current_workdir = dirname;
}

//! Processes click on active label with directory names embedded.

void FileTreeView::onLabelClick(const QString &link)
{
  SetCurrentDir(link);
}

//! Processed double clicks.

void FileTreeView::onTreeDoubleClick(const QModelIndex &index)
{
  qDebug() << "onTreeDoubleClick";
  QFileInfo info(m_file_system_model->filePath(index));

  if (info.isFile())
  {
    SetCurrentDir(info.dir().path());  // directory where clicked file is located
  }
  else
  {
    SetCurrentDir(info.filePath()); // directory which was clicked
  }

  if (IsProcedureFile(info))
  {
    emit procedureFileDoubleClicked(info.filePath());
  }
}

void FileTreeView::onTreeSingleClick(const QModelIndex &index)
{
  QFileInfo info(m_file_system_model->filePath(index));
  qDebug() << "onTreeSingleClick" << info.fileName() << info.filePath();

  if (info.isFile() && info.completeSuffix().toLower() == QStringLiteral("xml"))
  {
    if (m_current_xml_file != info.filePath())
    {
      qDebug() << "onTreeSingleClick -> XML file";
      m_current_xml_file = info.filePath();
      emit procedureFileClicked(m_current_xml_file);
    }
  }
}

//! Write widget settings to file.
void FileTreeView::writeSettings()
{
  QSettings settings;
  settings.setValue(GetWorkdirSettingName(), m_current_workdir);
}

//! Reads widget settings from file.
void FileTreeView::readSettings()
{
  QSettings settings;
  m_current_workdir = QDir::homePath();

  if (settings.contains(GetWorkdirSettingName()))
  {
    m_current_workdir = settings.value(GetWorkdirSettingName()).toString();
  }
}

}  // namespace sequi
