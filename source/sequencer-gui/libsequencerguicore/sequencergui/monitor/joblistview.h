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

#ifndef SEQUENCERGUI_MONITOR_JOBLISTVIEW_H
#define SEQUENCERGUI_MONITOR_JOBLISTVIEW_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

namespace mvvm
{
class ViewModel;
}

class QTreeView;

namespace sequencergui
{
class SequencerModel;
class ProcedureItem;

//! List view with top-level ProcedureItem's.
//! Occupy upper left corner of SequencerMonitorView.
//! FIXME almost full duplication of OpenDocumentsWidget

class JobListView : public CollapsibleWidget
{
  Q_OBJECT

public:
  explicit JobListView(QWidget* parent = nullptr);
  ~JobListView() override;

  void SetModel(SequencerModel* model);

  std::vector<ProcedureItem*> GetSelectedProcedures() const;
  ProcedureItem* GetSelectedProcedure();

  void SetSelectedProcedure(ProcedureItem* procedure);

signals:
  void createNewProcedureRequest();
  void procedureSelected(sequencergui::ProcedureItem* procedureItem);

private:
  void SetupToolBar();
  void onTreeSingleClick(const QModelIndex& index);
  void onRemoveSelectedRequest();

  QTreeView* m_tree_view{nullptr};
  SequencerModel* m_model{nullptr};
  std::unique_ptr<mvvm::ViewModel> m_view_model;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_JOBLISTVIEW_H
