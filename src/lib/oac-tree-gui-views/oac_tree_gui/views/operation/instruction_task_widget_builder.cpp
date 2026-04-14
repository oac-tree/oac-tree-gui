/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_task_widget_builder.h"

#include "task_widget.h"

#include <oac_tree_gui/model/instruction_item.h>

#include <stack>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief The StackNode helper struct to populate stack.
 */
struct StackNode
{
  const InstructionItem& item;
  TaskWidget& widget;
};

QString GetInstructionLabel(const InstructionItem& item)
{
  return QString::fromStdString(item.GetDisplayName());
}

}  // namespace

std::unique_ptr<TaskWidget> InstructionTaskWidgetBuilder::CreateTaskWidget(
    const InstructionItem& root_instruction)
{
  auto result = std::make_unique<TaskWidget>(GetInstructionLabel(root_instruction));

  std::stack<StackNode> stack;

  stack.push({root_instruction, *result});
  m_instruction_to_widget[&root_instruction] = result.get();

  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    for (const auto& child_instruction : node.item.GetInstructions())
    {
      auto child_widget = node.widget.CreateAndAddChild(GetInstructionLabel(*child_instruction));
      m_instruction_to_widget[child_instruction] = child_widget;
      stack.push({*child_instruction, *child_widget});
    }
  }

  return result;
}

TaskWidget* InstructionTaskWidgetBuilder::FindWidgetForInstruction(const InstructionItem* item)
{
  auto it = m_instruction_to_widget.find(item);
  return it == m_instruction_to_widget.end() ? nullptr : it->second;
}

std::size_t InstructionTaskWidgetBuilder::GetInstructionCount() const
{
  return m_instruction_to_widget.size();
}

}  // namespace oac_tree_gui
