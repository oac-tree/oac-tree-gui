/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_tree_selection_controller.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/viewmodel/viewmodel_helper.h>
#include <sequencergui/widgets/tree_helper.h>

#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QItemSelection>
#include <QItemSelectionModel>
#include <QTreeView>

namespace sequencergui
{

InstructionTreeSelectionController::InstructionTreeSelectionController(QTreeView *tree_view,
                                                                       QObject *parent)
    : QObject(parent), m_tree_view(tree_view)
{
}

void InstructionTreeSelectionController::SetInstructionContainer(
    InstructionContainerItem *instruction_container)
{
  m_instruction_container = instruction_container;
}

mvvm::SessionItem *InstructionTreeSelectionController::FindVisibleInstruction(
    const mvvm::SessionItem *item)
{
  auto indexes = GetViewModel()->GetIndexOfSessionItem(item);
  if (!indexes.empty())
  {
    auto visible_index = FindVisibleCandidate(*m_tree_view, indexes.at(0));
    return GetViewModel()->GetSessionItemFromIndex(visible_index);
  }

  return nullptr;
}

InstructionTreeSelectionController::~InstructionTreeSelectionController() = default;

void InstructionTreeSelectionController::SetSelected(const InstructionItem &item)
{
  auto indexes = GetViewModel()->GetIndexOfSessionItem(&item);
  if (!indexes.empty())
  {
    SetSelected(FindVisibleCandidate(*m_tree_view, indexes.at(0)));
  }
}

void InstructionTreeSelectionController::SetDefaultExpandState()
{
  auto viewmodel = GetViewModel();

  if (!m_instruction_container)
  {
    throw RuntimeException("Instruction container is not initialised");
  }

  auto on_index = [this, viewmodel](const auto &index)
  {
    if (!index.isValid())
    {
      return;
    }

    if (auto instruction =
            mvvm::utils::GetItemFromView<InstructionItem>(viewmodel->itemFromIndex(index));
        instruction)
    {
      m_tree_view->setExpanded(index, !IsCollapsed(*instruction));
    }
  };
  IterateFirstColumn(*viewmodel, QModelIndex(), on_index);
}

void InstructionTreeSelectionController::SetSelected(const QModelIndex &index)
{
  auto selection_model = m_tree_view->selectionModel();

  QItemSelection selection;
  selection.push_back(QItemSelectionRange(index));
  auto flags = QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows;
  selection_model->select(selection, flags);
}

mvvm::ViewModel *InstructionTreeSelectionController::GetViewModel()
{
  auto result = dynamic_cast<mvvm::ViewModel *>(m_tree_view->model());

  if (!result)
  {
    throw RuntimeException("The viewmodel is not initialized");
  }

  return result;
}

}  // namespace sequencergui
