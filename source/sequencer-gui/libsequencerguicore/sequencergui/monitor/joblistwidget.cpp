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

#include "sequencergui/monitor/joblistwidget.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/procedureviewmodel.h"
#include "sequencergui/utils/styleutils.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"
#include "mvvm/viewmodel/viewmodelutils.h"

#include <QAction>
#include <QLabel>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{
JobListWidget::JobListWidget(QWidget *parent)
    : QWidget(parent)
    , m_new_procedure_action(new QAction)
    , m_remove_selected_button(new QAction)
    , m_tree_view(new QTreeView)
{
  setWindowTitle("JOBS");
  setToolTip("List of current sequencer running/idle procedures");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  SetupActions();

//  connect(m_tree_view, &QTreeView::clicked, this, &JobListWidget::onTreeSingleClick);

  m_tree_view->setRootIsDecorated(false);
  m_tree_view->setHeaderHidden(true);
}

JobListWidget::~JobListWidget() = default;

void JobListWidget::SetJobModel(SequencerModel *model)
{
  m_model = model;
  m_view_model = std::make_unique<ProcedureViewModel>(model);
  m_view_model->SetRootSessionItem(model->GetProcedureContainer());
  m_tree_view->setModel(m_view_model.get());
}

//! FIXME restore
//ProcedureItem *JobListWidget::GetSelectedProcedure()
//{
//  auto selected = GetSelectedProcedures();
//  return selected.empty() ? nullptr : selected.front();
//}

//void JobListWidget::SetSelectedProcedure(ProcedureItem *procedure)
//{
//  auto indexes = m_view_model->GetIndexOfSessionItem(procedure);
//  if (!indexes.empty())
//  {
//    m_tree_view->selectionModel()->select(indexes.at(0), QItemSelectionModel::SelectCurrent);
//  }
//}

//! FIXME restore
//std::vector<ProcedureItem *> JobListWidget::GetSelectedProcedures() const
//{
//  std::vector<mvvm::SessionItem *> result;

//  if (!m_tree_view->selectionModel())
//  {
//    return {};
//  }
//  for (auto index : m_tree_view->selectionModel()->selectedIndexes())
//  {
//    auto procedure_item = m_view_model->GetSessionItemFromIndex(index);

//! FIXME restore
//    result.push_back(const_cast<mvvm::SessionItem *>(procedure_item));
//  }
//  return mvvm::utils::CastedItems<ProcedureItem>(result);
//}

void JobListWidget::SetupActions()
{
//  m_new_procedure_action->setIcon(StyleUtils::GetIcon("file-plus-outline.svg"));
//  connect(m_new_procedure_action, &QAction::triggered, this,
//          &JobListWidget::createNewProcedureRequest);
//  addAction(m_new_procedure_action);

//  m_remove_selected_button->setIcon(StyleUtils::GetIcon("beaker-remove-outline.svg"));
//  connect(m_remove_selected_button, &QAction::triggered, this,
//          &JobListWidget::onRemoveSelectedRequest);
//  addAction(m_remove_selected_button);
}

//void JobListWidget::onTreeSingleClick(const QModelIndex &index)
//{
//  if (!index.isValid())
//  {
//    return;
//  }

//  if (auto item = mvvm::utils::GetItemFromView<ProcedureItem>(m_view_model->itemFromIndex(index));
//      item)
//  {
//    emit procedureSelected(const_cast<ProcedureItem *>(item));
//  }
//}

//void JobListWidget::onRemoveSelectedRequest()
//{
//  for (auto procedure_item : GetSelectedProcedures())
//  {
//    m_model->RemoveItem(procedure_item);
//  }
//}

}  // namespace sequencergui
