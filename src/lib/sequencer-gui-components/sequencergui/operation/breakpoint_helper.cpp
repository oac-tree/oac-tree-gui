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

#include "breakpoint_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/iterate_helper.h>
#include <sequencergui/model/sequencer_item_helper.h>

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/model_utils.h>

#include <map>

namespace sequencergui
{

BreakpointStatus GetBreakpointStatus(const InstructionItem &item)
{
  return static_cast<BreakpointStatus>(GetBreakpointItem(item)->Data<int>());
}

void SetBreakpointStatus(const InstructionItem &item, BreakpointStatus status)
{
  auto breakpoint_property = GetBreakpointItem(item);
  breakpoint_property->SetData(static_cast<int>(status));
}

void ToggleBreakpointStatus(const InstructionItem &item)
{
  static const std::map<BreakpointStatus, BreakpointStatus> transitions = {
      {BreakpointStatus::kNotSet, BreakpointStatus::kSet},
      {BreakpointStatus::kSet, BreakpointStatus::kDisabled},
      {BreakpointStatus::kDisabled, BreakpointStatus::kNotSet}};
  auto new_status = transitions.at(GetBreakpointStatus(item));
  SetBreakpointStatus(item, new_status);
}

std::vector<BreakpointInfo> CollectBreakpointInfo(const InstructionItem &item)
{
  const auto *model = item.GetModel();
  if (!model)
  {
    throw LogicErrorException("Item should belong to a model");
  }

  std::vector<BreakpointInfo> result;

  auto save_breakpoint = [&result](const InstructionItem *item)
  {
    if (auto status = GetBreakpointStatus(*item); status != BreakpointStatus::kNotSet)
    {
      result.push_back({status, mvvm::utils::PathFromItem(item)});
    }
  };
  IterateInstruction<const InstructionItem *>(&item, save_breakpoint);

  return result;
}

std::vector<BreakpointInfo> CollectBreakpointInfo(const InstructionContainerItem &container)
{
  std::vector<BreakpointInfo> result;

  for (auto instruction : container.GetInstructions())
  {
    auto info = CollectBreakpointInfo(*instruction);
    std::copy(info.begin(), info.end(), std::back_inserter(result));
  }

  return result;
}

void SetBreakpointsFromInfo(const std::vector<BreakpointInfo> &info, InstructionItem &item)
{
  const auto *model = item.GetModel();
  if (!model)
  {
    throw LogicErrorException("Item should belong to a model");
  }

  for (const auto &[status, path] : info)
  {
    if (auto instruction =
            dynamic_cast<const InstructionItem *>(mvvm::utils::ItemFromPath(*model, path));
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

void SetBreakpointsFromInfo(const std::vector<BreakpointInfo> &info,
                            InstructionContainerItem &container)
{
  for (auto instruction : container.GetInstructions())
  {
    SetBreakpointsFromInfo(info, *instruction);
  }
}

}  // namespace sequencergui
