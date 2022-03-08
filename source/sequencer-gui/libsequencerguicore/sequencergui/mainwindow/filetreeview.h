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

#ifndef SEQUENCERGUI_MAINWINDOW_FILETREEVIEW_H
#define SEQUENCERGUI_MAINWINDOW_FILETREEVIEW_H

#include <QWidget>

class QTreeView;
class QFileSystemModel;
class QLabel;

namespace sequencergui
{

//! Simple file browser with bread crumb on top, and tree view at the bottom.

class FileTreeView : public QWidget
{
  Q_OBJECT

public:
  explicit FileTreeView(QWidget* parent = nullptr);
  ~FileTreeView() override;

  void SetCurrentDir(const QString& dirname);

signals:
  void procedureFileClicked(const QString& filename);
  void procedureFileDoubleClicked(const QString& filename);

private:
  void onLabelClick(const QString& link);
  void onTreeDoubleClick(const QModelIndex& index);
  void onTreeSingleClick(const QModelIndex& index);
  void writeSettings();
  void readSettings();

  QFileSystemModel* m_file_system_model{nullptr};
  QTreeView* m_tree_view{nullptr};
  QLabel* m_path_label{nullptr};
  QString m_current_workdir;
  QString m_current_xml_file;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_FILETREEVIEW_H
