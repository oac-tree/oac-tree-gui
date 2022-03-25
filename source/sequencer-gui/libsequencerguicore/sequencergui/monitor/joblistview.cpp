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

#include "sequencergui/monitor/joblistview.h"

#include "sequencergui/mainwindow/explorernarrowtoolbar.h"
#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"
#include "mvvm/viewmodel/viewmodelutils.h"

#include "sequencergui/monitor/procedureviewmodel.h"

#include <QLabel>
#include <QTreeView>
#include <QSplitter>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{
JobListView::JobListView(QWidget *parent)
    : CollapsibleWidget(parent), m_tree_view(new QTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  SetupToolBar();

  connect(m_tree_view, &QTreeView::clicked, this, &JobListView::onTreeSingleClick);

  m_tree_view->setRootIsDecorated(false);
}

JobListView::~JobListView() = default;

void JobListView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_view_model = std::make_unique<ProcedureViewModel>(model);
  m_view_model->SetRootSessionItem(model->GetProcedureContainer());
  m_tree_view->setModel(m_view_model.get());
}

ProcedureItem *JobListView::GetSelectedProcedure()
{
  auto selected = GetSelectedProcedures();
  return selected.empty() ? nullptr : selected.front();
}

void JobListView::SetSelectedProcedure(ProcedureItem *procedure)
{
  auto indexes = m_view_model->GetIndexOfSessionItem(procedure);
  if (!indexes.empty())
  {
    m_tree_view->selectionModel()->select(indexes.at(0), QItemSelectionModel::SelectCurrent);
  }
}

std::vector<ProcedureItem *> JobListView::GetSelectedProcedures() const
{
  std::vector<mvvm::SessionItem *> result;

  if (!m_tree_view->selectionModel())
  {
    return {};
  }
  for (auto index : m_tree_view->selectionModel()->selectedIndexes())
  {
    auto procedure_item = m_view_model->GetSessionItemFromIndex(index);

    result.push_back(const_cast<mvvm::SessionItem *>(procedure_item));
  }
  return mvvm::utils::CastedItems<ProcedureItem>(result);
}

void JobListView::SetupToolBar()
{
  SetText("PROCEDURES");

  auto tool_bar = GetToolBar();

  auto new_procedure_button = new QToolButton;
  new_procedure_button->setIcon(StyleUtils::GetIcon("file-plus-outline.svg"));
  connect(new_procedure_button, &QToolButton::clicked, this,
          &JobListView::createNewProcedureRequest);
  tool_bar->AddWidget(new_procedure_button);

  auto remove_selected_button = new QToolButton;
  remove_selected_button->setIcon(StyleUtils::GetIcon("beaker-remove-outline.svg"));
  connect(remove_selected_button, &QToolButton::clicked, this,
          &JobListView::onRemoveSelectedRequest);
  tool_bar->AddWidget(remove_selected_button);
}

void JobListView::onTreeSingleClick(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }

  if (auto item = mvvm::utils::GetItemFromView<ProcedureItem>(m_view_model->itemFromIndex(index));
      item)
  {
    emit procedureSelected(const_cast<ProcedureItem *>(item));
  }
}

void JobListView::onRemoveSelectedRequest()
{
  for (auto procedure_item : GetSelectedProcedures())
  {
    m_model->RemoveItem(procedure_item);
  }
}

}  // namespace sequencergui
