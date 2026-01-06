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

#include "breakpoint_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/iterate_helper.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/model_utils.h>

#include <map>

namespace oac_tree_gui
{

BreakpointStatus GetBreakpointStatus(const InstructionItem& item)
{
  return static_cast<BreakpointStatus>(GetBreakpointItem(item)->Data<std::int32_t>());
}

void SetBreakpointStatus(const InstructionItem& item, BreakpointStatus status)
{
  auto breakpoint_property = GetBreakpointItem(item);
  (void)breakpoint_property->SetData(static_cast<std::int32_t>(status));
}

void ToggleBreakpointStatus(const InstructionItem& item)
{
  static const std::map<BreakpointStatus, BreakpointStatus> transitions = {
      {BreakpointStatus::kNotSet, BreakpointStatus::kSet},
      {BreakpointStatus::kSet, BreakpointStatus::kDisabled},
      {BreakpointStatus::kDisabled, BreakpointStatus::kNotSet}};
  auto new_status = transitions.at(GetBreakpointStatus(item));
  SetBreakpointStatus(item, new_status);
}

std::vector<BreakpointInfo> CollectBreakpointInfo(const InstructionItem& item)
{
  const auto* model = item.GetModel();
  if (model == nullptr)
  {
    throw LogicErrorException("Item should belong to a model");
  }

  std::vector<BreakpointInfo> result;

  auto save_breakpoint = [&result](const InstructionItem* item)
  {
    if (auto status = GetBreakpointStatus(*item); status != BreakpointStatus::kNotSet)
    {
      result.push_back({status, mvvm::utils::PathFromItem(item)});
    }
  };
  IterateInstruction<const InstructionItem*>(&item, save_breakpoint);

  return result;
}

std::vector<BreakpointInfo> CollectBreakpointInfo(const InstructionContainerItem& container)
{
  std::vector<BreakpointInfo> result;

  for (auto instruction : container.GetInstructions())
  {
    auto info = CollectBreakpointInfo(*instruction);
    (void)std::copy(info.begin(), info.end(), std::back_inserter(result));
  }

  return result;
}

void SetBreakpointsFromInfo(const std::vector<BreakpointInfo>& info, InstructionItem& item)
{
  const auto* model = item.GetModel();
  if (model == nullptr)
  {
    throw LogicErrorException("Item should belong to a model");
  }

  for (const auto& [status, path] : info)
  {
    if (auto instruction =
            dynamic_cast<const InstructionItem*>(mvvm::utils::ItemFromPath(*model, path));
        instruction)
    {
      SetBreakpointStatus(*instruction, status);
    }
    else
    {
      throw RuntimeException("Can't find instruction");
    }
  }
}

void SetBreakpointsFromInfo(const std::vector<BreakpointInfo>& info,
                            InstructionContainerItem& container)
{
  for (auto instruction : container.GetInstructions())
  {
    SetBreakpointsFromInfo(info, *instruction);
  }
}

}  // namespace oac_tree_gui
