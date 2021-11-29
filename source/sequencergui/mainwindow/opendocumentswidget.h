/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_OPENDOCUMENTSWIDGET_H
#define SEQUENCERGUI_MAINWINDOW_OPENDOCUMENTSWIDGET_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

namespace ModelView
{
class ViewModel;
}

class QListView;

namespace sequi
{
class SequencerModel;
class ProcedureItem;
class ExplorerNarrowToolBar;

//! Vertical panel located on the left of XMLTreeView

class OpenDocumentsWidget : public CollapsibleWidget
{
  Q_OBJECT

public:
  OpenDocumentsWidget(QWidget* parent = nullptr);
  ~OpenDocumentsWidget();

  void SetModel(SequencerModel* model);

  std::vector<ProcedureItem *> GetSelectedProcedures() const;
  ProcedureItem* GetSelectedProcedure();

  void SetSelectedProcedure(ProcedureItem* procedure);

signals:
  void createNewProcedureRequest();
  void procedureSelected(ProcedureItem* procedureItem);

private:
  void SetupToolBar();
  void onTreeSingleClick(const QModelIndex& index);
  void onRemoveSelectedRequest();

  QListView* m_list_view{nullptr};
  SequencerModel* m_model{nullptr};
  std::unique_ptr<ModelView::ViewModel> m_view_model;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_OPENDOCUMENTSWIDGET_H
