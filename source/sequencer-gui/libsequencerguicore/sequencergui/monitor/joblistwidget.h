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

#ifndef SEQUENCERGUI_MONITOR_JOBLISTWIDGET_H
#define SEQUENCERGUI_MONITOR_JOBLISTWIDGET_H

#include <QWidget>

namespace mvvm
{
class ViewModel;
}

class QTreeView;
class QAction;

namespace sequencergui
{
class JobModel;
class JobItem;

//! List view with top-level ProcedureItem's.
//! Occupy upper left corner of SequencerMonitorView.
//! FIXME almost full duplication of ProcedureListWidget

class JobListWidget : public QWidget
{
  Q_OBJECT

public:
  explicit JobListWidget(QWidget* parent = nullptr);
  ~JobListWidget() override;

  void SetJobModel(JobModel* model);

  //! FIXME restore
//  std::vector<ProcedureItem*> GetSelectedProcedures() const;
//  ProcedureItem* GetSelectedProcedure();

  //! FIXME restore
//  void SetSelectedProcedure(ProcedureItem* procedure);

//signals:
//  void createNewProcedureRequest();
//  void procedureSelected(sequencergui::ProcedureItem* procedureItem);

private:
  void SetupActions();
  void onTreeSingleClick(const QModelIndex& index);
  void onRemoveSelectedRequest();

  QAction* m_new_procedure_action{nullptr};
  QAction* m_remove_selected_button{nullptr};

  QTreeView* m_tree_view{nullptr};
  JobModel* m_model{nullptr};
  std::unique_ptr<mvvm::ViewModel> m_view_model;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_JOBLISTWIDGET_H
