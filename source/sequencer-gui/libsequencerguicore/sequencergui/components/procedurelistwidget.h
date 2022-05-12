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

#ifndef SEQUENCERGUI_COMPONENTS_PROCEDURELISTWIDGET_H
#define SEQUENCERGUI_COMPONENTS_PROCEDURELISTWIDGET_H

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
class ProcedureList;

//! List view for ProcedureItem with selection abilities.

class ProcedureListWidget : public QWidget
{
  Q_OBJECT

public:
  enum Actions
  {
    kCreateNew = 1,
    kRemoveSelected = 2
  };

  explicit ProcedureListWidget(QWidget* parent = nullptr);
  ~ProcedureListWidget() override;

  void SetModel(SequencerModel* model);

  ProcedureItem* GetSelectedProcedure();
  std::vector<ProcedureItem*> GetSelectedProcedures() const;

  void SetSelectedProcedure(ProcedureItem* procedure);

  QListView* GetListView();

  mvvm::ViewModel* GetViewModel();

  void SetupActions(int action_flag);

signals:
  void ProcedureSelected(sequencergui::ProcedureItem* procedure_item);
  void CreateNewProcedureRequest();
  void RemoveProcedureRequest(sequencergui::ProcedureItem* procedure_item);

private:
  void OnRemoveSelectedRequest();

  QAction* m_new_procedure_action{nullptr};
  QAction* m_remove_selected_action{nullptr};

  ProcedureList* m_list_view{nullptr};
  SequencerModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_PROCEDURELISTWIDGET_H
