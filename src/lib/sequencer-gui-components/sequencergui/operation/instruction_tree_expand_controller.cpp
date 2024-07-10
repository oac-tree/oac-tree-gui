/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_tree_expand_controller.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/viewmodel/viewmodel_helper.h>
#include <sup/gui/widgets/tree_helper.h>

#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QTreeView>

namespace sequencergui
{

InstructionTreeExpandController::InstructionTreeExpandController(QTreeView *tree_view,
                                                                 QObject *parent)
    : QObject(parent), m_tree_view(tree_view)
{
  connect(tree_view, &QTreeView::collapsed, this, [this](auto) { emit VisibilityHasChanged(); });
  connect(tree_view, &QTreeView::expanded, this, [this](auto) { emit VisibilityHasChanged(); });
}

void InstructionTreeExpandController::SetInstructionContainer(
    InstructionContainerItem *instruction_container)
{
  m_instruction_container = instruction_container;
}

void InstructionTreeExpandController::SaveSelectionRequest(
    const std::vector<InstructionItem *> &instructions)
{
  m_selection_preferences = instructions;
}

std::vector<mvvm::SessionItem *> InstructionTreeExpandController::GetInstructionsToSelect() const
{
  std::vector<mvvm::SessionItem *> result;
  result.reserve(m_selection_preferences.size());
  for (auto item : m_selection_preferences)
  {
    result.push_back(FindVisibleInstruction(item));
  }

  return result;
}

mvvm::SessionItem *InstructionTreeExpandController::FindVisibleInstruction(
    const mvvm::SessionItem *item) const
{
  auto indexes = GetViewModel()->GetIndexOfSessionItem(item);
  if (!indexes.empty())
  {
    auto visible_index = sup::gui::FindVisibleCandidate(*m_tree_view, indexes.at(0));
    return GetViewModel()->GetSessionItemFromIndex(visible_index);
  }

  return nullptr;
}

InstructionTreeExpandController::~InstructionTreeExpandController() = default;

void InstructionTreeExpandController::SetDefaultExpandState()
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

mvvm::ViewModel *InstructionTreeExpandController::GetViewModel() const
{
  auto result = dynamic_cast<mvvm::ViewModel *>(m_tree_view->model());

  if (!result)
  {
    throw RuntimeException("The viewmodel is not initialized");
  }

  return result;
}

}  // namespace sequencergui
