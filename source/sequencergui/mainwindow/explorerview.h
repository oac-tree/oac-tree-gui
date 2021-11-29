/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_EXPLORERVIEW_H
#define SEQUENCERGUI_MAINWINDOW_EXPLORERVIEW_H

#include <QWidget>

class QSplitter;

namespace sequi
{
class FileTreeView;
class ExplorerToolBar;
class OpenDocumentsWidget;
class SequencerModel;
class ProcedureItem;

//! Vertical panel located on the left of XMLTreeView

class ExplorerView : public QWidget
{
  Q_OBJECT

public:
  ExplorerView(QWidget* parent = nullptr);
  ~ExplorerView();

  void SetModel(SequencerModel* model);

signals:
  void procedureFileClicked(const QString& filename);
  void procedureFileDoubleClicked(const QString& filename);
  void createNewProcedureRequest();
  void sratchpadProcedureSelected(ProcedureItem* procedureItem);

private:
  ExplorerToolBar* m_tool_bar{nullptr};

  QSplitter* m_splitter{nullptr};
  FileTreeView* m_file_tree_view{nullptr};  
  OpenDocumentsWidget* m_open_documents_widget{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_EXPLORERVIEW_H
