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

#include "sequencergui/mainwindow/collapsibletoolbar.h"
#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

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
{
  setWindowTitle("PROCEDURES");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_list_view);

  SetupActions();

  connect(m_list_view, &QListView::clicked, this, &ProcedureListView::OnSingleClick,
          Qt::UniqueConnection);
}

ProcedureListView::~ProcedureListView() = default;

void ProcedureListView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_view_model = std::make_unique<mvvm::TopItemsViewModel>(model);
  m_view_model->SetRootSessionItem(model->GetProcedureContainer());
  m_list_view->setModel(m_view_model.get());

  connect(m_list_view->selectionModel(), &QItemSelectionModel::selectionChanged, this,
          &ProcedureListView::OnSelectionChanged, Qt::UniqueConnection);
}

ProcedureItem *ProcedureListView::GetSelectedProcedure()
{
  auto selected = GetSelectedProcedures();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<ProcedureItem *> ProcedureListView::GetSelectedProcedures() const
{
  std::vector<mvvm::SessionItem *> result;

  if (!m_list_view->selectionModel())
  {
    return {};
  }
  for (auto index : m_list_view->selectionModel()->selectedIndexes())
  {
    auto procedure_item = m_view_model->GetSessionItemFromIndex(index);

    result.push_back(const_cast<mvvm::SessionItem *>(procedure_item));
  }
  return mvvm::utils::CastedItems<ProcedureItem>(mvvm::utils::UniqueItems(result));
}

void ProcedureListView::SetSelectedProcedure(ProcedureItem *procedure)
{
  SetSelectedProcedures({procedure});
}

void ProcedureListView::SetSelectedProcedures(std::vector<ProcedureItem *> procedures)
{
  if (!m_view_model)
  {
    return;
  }

  m_list_view->selectionModel()->clearSelection();
  QItemSelection selection;
  for (auto item : procedures)
  {
    for (auto index : m_view_model->GetIndexOfSessionItem(item))
    {
      selection.push_back(QItemSelectionRange(index));
    }
  }
  m_list_view->selectionModel()->select(selection, QItemSelectionModel::Select);
}

void ProcedureListView::SetupActions()
{
  m_new_procedure_action->setIcon(StyleUtils::GetIcon("file-plus-outline.svg"));
  connect(m_new_procedure_action, &QAction::triggered, this,
          &ProcedureListView::CreateNewProcedureRequest);
  addAction(m_new_procedure_action);

  m_remove_selected_action->setIcon(StyleUtils::GetIcon("beaker-remove-outline.svg"));
  connect(m_remove_selected_action, &QAction::triggered, this,
          &ProcedureListView::OnRemoveSelectedRequest);
  addAction(m_remove_selected_action);
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

void ProcedureListView::OnRemoveSelectedRequest()
{
  for (auto procedure_item : GetSelectedProcedures())
  {
    m_model->RemoveItem(procedure_item);
  }
}

void ProcedureListView::OnSelectionChanged(const QItemSelection &, const QItemSelection &deselected)
{
  if (GetSelectedProcedures().empty())  // selections has gone
  {
    emit ProcedureSelected(nullptr);
    return;
  }
  for (auto item : GetSelectedProcedures())
  {
    emit ProcedureSelected(item);
  }
}

}  // namespace sequencergui
