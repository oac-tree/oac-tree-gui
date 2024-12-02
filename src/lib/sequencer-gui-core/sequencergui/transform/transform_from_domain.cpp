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

#include "transform_from_domain.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/model/sequencer_item_includes.h>

#include <mvvm/model/function_types.h>
#include <mvvm/model/item_catalogue.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/workspace.h>

namespace sequencergui
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
  result->RegisterItem<SystemCallInstructionItem>();
  result->RegisterItem<LogInstructionItem>();

  return result;
}

std::map<sup::sequencer::JobState, RunnerStatus> GetRunnerStatusMap()
{
  using sup::sequencer::JobState;
  std::map<sup::sequencer::JobState, RunnerStatus> result = {
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

RunnerStatus GetRunnerStatusFromDomain(sup::sequencer::JobState job_state)
{
  static const auto kRunnerStatusMap = GetRunnerStatusMap();
  auto iter = kRunnerStatusMap.find(job_state);
  if (iter == kRunnerStatusMap.end())
  {
    throw RuntimeException("Can't convert sup::sequencer::JobState");
  }
  return iter->second;
}

}  // namespace sequencergui
