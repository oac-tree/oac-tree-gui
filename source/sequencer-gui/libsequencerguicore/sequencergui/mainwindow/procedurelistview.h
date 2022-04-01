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

#ifndef SEQUENCERGUI_MAINWINDOW_PROCEDURELISTVIEW_H
#define SEQUENCERGUI_MAINWINDOW_PROCEDURELISTVIEW_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

namespace mvvm
{
class ViewModel;
}

class QListView;
class QItemSelection;

namespace sequencergui
{
class SequencerModel;
class ProcedureItem;

//! Vertical panel located on the left of XMLTreeView

class ProcedureListView : public CollapsibleWidget
{
  Q_OBJECT

public:
  explicit ProcedureListView(QWidget* parent = nullptr);
  ~ProcedureListView() override;

  void SetModel(SequencerModel* model);

  ProcedureItem* GetSelectedProcedure();
  std::vector<ProcedureItem*> GetSelectedProcedures() const;

  void SetSelectedProcedure(ProcedureItem* procedure);
  void SetSelectedProcedures(std::vector<ProcedureItem*> procedures);

signals:
  void CreateNewProcedureRequest();
  void ProcedureClicked(sequencergui::ProcedureItem* procedureItem);
  void ProcedureSelected(sequencergui::ProcedureItem* procedureItem);

private:
  void SetupToolBar();
  void OnSingleClick(const QModelIndex& index);
  void OnRemoveSelectedRequest();
  void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

  QListView* m_list_view{nullptr};
  SequencerModel* m_model{nullptr};
  std::unique_ptr<mvvm::ViewModel> m_view_model;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_PROCEDURELISTVIEW_H
