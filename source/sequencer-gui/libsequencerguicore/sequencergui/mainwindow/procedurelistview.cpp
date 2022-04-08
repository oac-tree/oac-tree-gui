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

#include "sequencergui/mainwindow/procedurelistview.h"

#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/viewmodel/selectionmodel.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"
#include "mvvm/viewmodel/viewmodelutils.h"

#include <QAction>
#include <QItemSelectionModel>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>

namespace sequencergui
{
ProcedureListView::ProcedureListView(QWidget *parent)
    : QWidget(parent)
    , m_new_procedure_action(new QAction)
    , m_remove_selected_action(new QAction)
    , m_list_view(new QListView)
    , m_selection_model(std::make_unique<SelectionModel>())
{
  setWindowTitle("PROCEDURES");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_list_view);
}

ProcedureListView::~ProcedureListView() = default;

void ProcedureListView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_view_model = std::make_unique<mvvm::TopItemsViewModel>(model);
  m_view_model->SetRootSessionItem(model->GetProcedureContainer());

  m_selection_model->SetViewModel(m_view_model.get());

  m_list_view->setModel(m_view_model.get());
  m_list_view->setSelectionModel(m_selection_model.get());

  connect(m_selection_model.get(), &SelectionModel::SelectedItemChanged, this,
          [this](auto) { emit ProcedureSelected(GetSelectedProcedure()); });

  connect(m_list_view, &QListView::clicked, this, &ProcedureListView::OnSingleClick,
          Qt::UniqueConnection);
}

ProcedureItem *ProcedureListView::GetSelectedProcedure()
{
  auto selected = GetSelectedProcedures();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<ProcedureItem *> ProcedureListView::GetSelectedProcedures() const
{
  std::vector<ProcedureItem *> result;
  auto selected = m_selection_model->GetSelectedItems();
  auto on_item = [](auto it)
  { return dynamic_cast<ProcedureItem *>(const_cast<mvvm::SessionItem *>(it)); };
  std::transform(selected.begin(), selected.end(), std::back_inserter(result), on_item);
  return result;
}

void ProcedureListView::SetSelectedProcedure(ProcedureItem *procedure)
{
  m_selection_model->SetSelectedItem(procedure);
}

QListView *ProcedureListView::GetListView()
{
  return m_list_view;
}

mvvm::ViewModel *ProcedureListView::GetViewModel()
{
  return m_view_model.get();
}

void ProcedureListView::SetupActions(int action_flag)
{
  if (action_flag & kCreateNew)
  {
    m_new_procedure_action->setIcon(StyleUtils::GetIcon("file-plus-outline.svg"));
    connect(m_new_procedure_action, &QAction::triggered, this,
            &ProcedureListView::CreateNewProcedureRequest);
    addAction(m_new_procedure_action);
  }

  if (action_flag & kRemoveSelected)
  {
    m_remove_selected_action->setIcon(StyleUtils::GetIcon("beaker-remove-outline.svg"));
    auto on_remove = [this]() { emit RemoveProcedureRequest(GetSelectedProcedure()); };
    connect(m_remove_selected_action, &QAction::triggered, this, on_remove);
    addAction(m_remove_selected_action);
  }
}

void ProcedureListView::OnSingleClick(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }

  if (auto item = mvvm::utils::GetItemFromView<ProcedureItem>(m_view_model->itemFromIndex(index));
      item)
  {
    emit ProcedureClicked(const_cast<ProcedureItem *>(item));
  }
}

}  // namespace sequencergui
