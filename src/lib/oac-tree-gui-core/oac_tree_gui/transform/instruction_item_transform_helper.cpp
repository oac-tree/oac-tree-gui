/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_item_transform_helper.h"

#include <oac_tree_gui/domain/domain_automation_helper.h>
#include <oac_tree_gui/model/instruction_info_item.h>
#include <oac_tree_gui/transform/transform_from_domain.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/instruction_info_utils.h>
#include <sup/oac-tree/instruction_map.h>

#include <stack>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief The InstructionInfoStackNode structs stores stack information during traversing of
 * InstructionInfo object.
 */
struct InstructionInfoStackNode
{
  const sup::oac_tree::InstructionInfo& info;
  oac_tree_gui::InstructionItem& item;
};

/**
 * @brief Creates instruction tree from provided domain information.
 *
 * Depending on light_tree flag, the resulting tree can be either InstructionInfoItem based, or will
 * contain a proper InstructionItem hierarchy.
 *
 * @param info Instruction info from the domain
 * @param info_based Output tree will be based of InstructionInfoItem if true
 * @return Instruction tree.
 */
InstructionTree CreateInstructionItemTreeIntern(const sup::oac_tree::InstructionInfo& info,
                                                bool light_tree)
{
  std::stack<InstructionInfoStackNode> stack;

  std::unique_ptr<InstructionItem> result =
      light_tree ? CreateInstructionInfoItem(info) : CreateInstructionItem(info);
  std::map<const InstructionItem*, std::size_t> item_to_index;

  stack.push({info, *result});
  item_to_index[result.get()] = info.GetIndex();

  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    for (auto child_info : node.info.Children())
    {
      auto child_item =
          light_tree ? CreateInstructionInfoItem(*child_info) : CreateInstructionItem(*child_info);
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

}  // namespace

std::unique_ptr<InstructionItem> CreateInstructionItem(const sup::oac_tree::InstructionInfo& info)
{
  auto result = oac_tree_gui::CreateInstructionItem(info.GetType());

  // We create domain instruction only to be able to use InitFromDomain mechanism for attribute
  // propagation.
  auto domain = CreateDomainInstruction(info);
  result->InitFromDomain(domain.get());

  return result;
}

std::unique_ptr<InstructionItem> CreateInstructionInfoItem(
    const sup::oac_tree::InstructionInfo& info)
{
  auto result = std::make_unique<InstructionInfoItem>();
  result->InitFromDomainInfo(info);

  return result;
}

InstructionTree CreateInstructionItemTree(const sup::oac_tree::InstructionInfo& info)
{
  return CreateInstructionItemTreeIntern(info, /*light_tree*/ false);
}

InstructionTree CreateInstructionInfoItemTree(const sup::oac_tree::InstructionInfo& info)
{
  return CreateInstructionItemTreeIntern(info, /*light_tree*/ true);
}

InstructionTree CreateInstructionItemTree(const sup::oac_tree::Instruction& instruction)
{
  const sup::oac_tree::InstructionMap instr_map{&instruction};
  auto instr_info = sup::oac_tree::utils::CreateInstructionInfoTree(instruction, instr_map);
  return CreateInstructionItemTree(*instr_info);
}

}  // namespace oac_tree_gui
