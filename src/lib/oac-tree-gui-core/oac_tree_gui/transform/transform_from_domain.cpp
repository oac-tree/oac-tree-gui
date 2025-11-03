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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "transform_from_domain.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_item_includes.h>

#include <mvvm/model/function_types.h>
#include <mvvm/model/item_catalogue.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/workspace.h>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Creates catalogue where VariableItem factory functions are registered against "Type" name
 * strings known by the sequencer.
 * @return
 */
std::unique_ptr<mvvm::ItemCatalogue<VariableItem>> CreateVariableItemCatalogue()
{
  auto result = std::make_unique<mvvm::ItemCatalogue<VariableItem>>();

  result->RegisterItem<LocalVariableItem>();
  result->RegisterItem<FileVariableItem>();
  result->RegisterItem<ChannelAccessVariableItem>();
  result->RegisterItem<PvAccessClientVariableItem>();
  result->RegisterItem<PvAccessServerVariableItem>();

  return result;
}

/**
 * @brief Creates catalogue where InstructionItem factory functions are registered against "Type"
 * name strings known by the sequencer.
 */
std::unique_ptr<mvvm::ItemCatalogue<InstructionItem>> CreateInstructionItemCatalogue()
{
  auto result = std::make_unique<mvvm::ItemCatalogue<InstructionItem>>();

  result->RegisterItem<IncludeItem>();
  result->RegisterItem<ParallelSequenceItem>();
  result->RegisterItem<RepeatItem>();
  result->RegisterItem<SequenceItem>();
  result->RegisterItem<WaitItem>();

  // instructions from sequencer-plugin-epics
  result->RegisterItem<ChannelAccessReadInstructionItem>();
  result->RegisterItem<ChannelAccessWriteInstructionItem>();
  result->RegisterItem<PvAccessReadInstructionItem>();
  result->RegisterItem<PvAccessWriteInstructionItem>();
  result->RegisterItem<RPCClientInstruction>();
  result->RegisterItem<LogInstructionItem>();

  return result;
}

std::map<sup::oac_tree::JobState, RunnerStatus> CreateRunnerStatusMap()
{
  using sup::oac_tree::JobState;
  const std::map<sup::oac_tree::JobState, RunnerStatus> result = {
      {JobState::kInitial, RunnerStatus::kInitial},
      {JobState::kPaused, RunnerStatus::kPaused},
      {JobState::kStepping, RunnerStatus::kStepping},
      {JobState::kRunning, RunnerStatus::kRunning},
      {JobState::kSucceeded, RunnerStatus::kSucceeded},
      {JobState::kFailed, RunnerStatus::kFailed},
      {JobState::kHalted, RunnerStatus::kHalted}};
  return result;
}

}  // namespace

std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type)
{
  static const auto catalogue = CreateVariableItemCatalogue();

  if (!catalogue->IsRegistered(domain_type))
  {
    auto result = std::make_unique<UniversalVariableItem>();
    result->SetDomainType(domain_type);
    return result;
  }

  return catalogue->Create(domain_type);
}

std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type)
{
  static const auto catalogue = CreateInstructionItemCatalogue();

  if (!catalogue->IsRegistered(domain_type))
  {
    auto result = std::make_unique<UniversalInstructionItem>();
    result->SetDomainType(domain_type);
    return result;
  }

  return catalogue->Create(domain_type);
}

RunnerStatus GetRunnerStatusFromDomain(sup::oac_tree::JobState job_state)
{
  static const auto kRunnerStatusMap = CreateRunnerStatusMap();
  auto iter = kRunnerStatusMap.find(job_state);
  if (iter == kRunnerStatusMap.end())
  {
    throw RuntimeException("Can't convert sup::oac_tree::JobState");
  }
  return iter->second;
}

}  // namespace oac_tree_gui
