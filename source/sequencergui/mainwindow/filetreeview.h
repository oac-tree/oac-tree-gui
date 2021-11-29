/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_FILETREEVIEW_H
#define SEQUENCERGUI_MAINWINDOW_FILETREEVIEW_H

#include <QWidget>

class QTreeView;
class QFileSystemModel;
class QLabel;

namespace sequi
{
//! Simple file browser with bread crumb on top, and tree view at the bottom.
class FileTreeView : public QWidget
{
  Q_OBJECT

public:
  FileTreeView(QWidget* parent = nullptr);
  ~FileTreeView();

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

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_FILETREEVIEW_H
