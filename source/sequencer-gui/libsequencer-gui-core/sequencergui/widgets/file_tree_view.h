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

#ifndef SEQUENCERGUI_WIDGETS_FILE_TREE_VIEW_H_
#define SEQUENCERGUI_WIDGETS_FILE_TREE_VIEW_H_

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
  void ProcedureFileClicked(const QString& filename);
  void ProcedureFileDoubleClicked(const QString& filename);

private:
  void OnLabelClick(const QString& link);
  void OnTreeDoubleClick(const QModelIndex& index);
  void OnTreeSingleClick(const QModelIndex& index);
  void WriteSettings();
  void ReadSettings();

  QFileSystemModel* m_file_system_model{nullptr};
  QTreeView* m_tree_view{nullptr};
  QLabel* m_path_label{nullptr};
  QString m_current_workdir;
  QString m_current_xml_file;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_FILE_TREE_VIEW_H_
