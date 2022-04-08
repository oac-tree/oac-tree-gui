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

#include <QWidget>

namespace mvvm
{
class ViewModel;
}

class QListView;
class QAction;

namespace sequencergui
{
class SequencerModel;
class ProcedureItem;
class SelectionModel;

//! List view for ProcedureItem with selection abilities.

class ProcedureListView : public QWidget
{
  Q_OBJECT

public:
  explicit ProcedureListView(QWidget* parent = nullptr);
  ~ProcedureListView() override;

  void SetModel(SequencerModel* model);

  ProcedureItem* GetSelectedProcedure();
  std::vector<ProcedureItem*> GetSelectedProcedures() const;

  void SetSelectedProcedure(ProcedureItem* procedure);

signals:
  void CreateNewProcedureRequest();
  void ProcedureClicked(sequencergui::ProcedureItem* procedure_item);
  void ProcedureSelected(sequencergui::ProcedureItem* procedure_item);

private:
  void SetupActions();
  void OnSingleClick(const QModelIndex& index);
  void OnRemoveSelectedRequest();

  QAction* m_new_procedure_action{nullptr};
  QAction* m_remove_selected_action{nullptr};

  QListView* m_list_view{nullptr};
  SequencerModel* m_model{nullptr};
  std::unique_ptr<SelectionModel> m_selection_model;
  std::unique_ptr<mvvm::ViewModel> m_view_model;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_PROCEDURELISTVIEW_H
