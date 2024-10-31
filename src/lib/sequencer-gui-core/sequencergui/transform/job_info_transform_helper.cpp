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

#include "job_info_transform_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_automation_helper.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_from_domain.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_info.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/variable_info.h>
#include <sup/sequencer/workspace_info.h>

#include <stack>

namespace
{

struct InstructionInfoStackNode
{
  const sup::sequencer::InstructionInfo& info;
  sequencergui::InstructionItem& item;
};

}  // namespace

namespace sequencergui
{

std::unique_ptr<InstructionItem> CreateInstructionItem(const sup::sequencer::InstructionInfo& info)
{
  auto result = sequencergui::CreateInstructionItem(info.GetType());

  // We create domain instruction only to be able to use InitFromDomain mechanism for attribute
  // propagation.
  auto domain = CreateDomainInstruction(info);
  result->InitFromDomain(domain.get());

  return result;
}

InstructionTree CreateInstructionItemTree(const sup::sequencer::InstructionInfo& info)
{
  std::stack<InstructionInfoStackNode> stack;

  std::unique_ptr<InstructionItem> result = CreateInstructionItem(info);
  std::map<const InstructionItem*, size_t> item_to_index;

  stack.push({info, *result});
  item_to_index[result.get()] = info.GetIndex();

  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    for (auto child_info : node.info.Children())
    {
      auto child_item = CreateInstructionItem(*child_info);
      auto child_item_ptr = child_item.get();
      item_to_index[child_item_ptr] = child_info->GetIndex();

      node.item.InsertItem(std::move(child_item), mvvm::TagIndex::Append());

      stack.push({*child_info, *child_item_ptr});
    }
  }

  std::vector<const InstructionItem*> index_list(item_to_index.size());
  std::for_each(item_to_index.begin(), item_to_index.end(),
                [&index_list](auto it) { index_list[it.second] = it.first; });

  return {std::move(result), std::move(index_list)};
}

std::unique_ptr<VariableItem> CreateVariableItem(const sup::sequencer::VariableInfo& info)
{
  auto result = sequencergui::CreateVariableItem(info.GetType());

  // We create domain variable only to be able to use InitFromDomain mechanism for attribute
  // propagation.
  auto domain = CreateDomainVariable(info);
  result->InitFromDomain(domain.get());

  return result;
}

std::vector<const VariableItem*> PopulateWorkspaceItem(const sup::sequencer::WorkspaceInfo& info,
                                                       WorkspaceItem* workspace_item)
{
  std::vector<const VariableItem*> index_to_variable_item;

  if (workspace_item->GetVariableCount() != 0)
  {
    throw RuntimeException("WorkspaceItem already contains some variables");
  }

  index_to_variable_item.resize(info.GetNumberOfVariables());

  for (const auto& [name, variable_info] : info.GetVariableInfos())
  {
    auto variable_item = CreateVariableItem(variable_info);
    auto variable_item_ptr = variable_item.get();
    index_to_variable_item[variable_info.GetIndex()] = variable_item_ptr;
    workspace_item->InsertItem(std::move(variable_item), mvvm::TagIndex::Append());
  }

  return index_to_variable_item;
}

}  // namespace sequencergui
