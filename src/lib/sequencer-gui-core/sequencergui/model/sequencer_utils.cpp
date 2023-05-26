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

#include "sequencer_utils.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/sequencer_item_includes.h>

#include <mvvm/model/item_catalogue.h>
#include <mvvm/model/item_manager.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/utils/container_utils.h>

#include <vector>

namespace
{

std::vector<std::string> GetDomainDecoratorNames()
{
  std::vector<std::string> result{sequencergui::domainconstants::kForceSuccessInstructionType,
                                  sequencergui::domainconstants::kIncludeInstructionType,
                                  sequencergui::domainconstants::kListenInstructionType,
                                  sequencergui::domainconstants::kRepeatInstructionType,
                                  sequencergui::domainconstants::kInverterInstructionType};
  return result;
}
}  // namespace

namespace sequencergui
{

bool IsCompoundInstruction(const InstructionItem *instruction)
{
  return instruction->GetTaggedItems()->HasTag(itemconstants::kChildInstructions);
}

bool IsDecoratorInstruction(const InstructionItem *instruction)
{
  static const std::vector<std::string> domain_names = GetDomainDecoratorNames();
  return mvvm::utils::Contains(domain_names, instruction->GetDomainType());
}

std::unique_ptr<mvvm::ItemCatalogue<mvvm::SessionItem>> CreateSequencerItemCatalogue()
{
  auto result = std::make_unique<mvvm::ItemCatalogue<mvvm::SessionItem>>();

  // instructions
  result->RegisterItem<ChoiceItem>();
  result->RegisterItem<ConditionItem>();
  result->RegisterItem<CopyItem>();
  result->RegisterItem<DecrementItem>();
  result->RegisterItem<FallbackItem>();
  result->RegisterItem<ForceSuccessItem>();
  result->RegisterItem<IncludeItem>();
  result->RegisterItem<IncrementItem>();
  result->RegisterItem<InputItem>();
  result->RegisterItem<InverterItem>();
  result->RegisterItem<ListenItem>();
  result->RegisterItem<MessageItem>();
  result->RegisterItem<OutputItem>();
  result->RegisterItem<ParallelSequenceItem>();
  result->RegisterItem<RepeatItem>();
  result->RegisterItem<SequenceItem>();
  result->RegisterItem<UserChoiceItem>();
  result->RegisterItem<VariableResetItem>();
  result->RegisterItem<WaitItem>();
  result->RegisterItem<UniversalInstructionItem>();

  // equality instructions
  result->RegisterItem<LessThanItem>();
  result->RegisterItem<LessThanOrEqualItem>();
  result->RegisterItem<EqualsItem>();
  result->RegisterItem<GreaterThanOrEqualItem>();
  result->RegisterItem<GreaterThanItem>();

  // sequencer-plugin-epics instructions
  result->RegisterItem<ChannelAccessReadInstructionItem>();
  result->RegisterItem<ChannelAccessWriteInstructionItem>();
  result->RegisterItem<PvAccessReadInstructionItem>();
  result->RegisterItem<PvAccessWriteInstructionItem>();
  result->RegisterItem<RPCClientInstruction>();
  result->RegisterItem<SystemCallInstructionItem>();
  result->RegisterItem<LogInstructionItem>();

  // variables
  result->RegisterItem<FileVariableItem>();
  result->RegisterItem<LocalVariableItem>();
  result->RegisterItem<UniversalVariableItem>();

  // variables sequencer-plugin-epics
  result->RegisterItem<ChannelAccessVariableItem>();
  result->RegisterItem<PvAccessClientVariableItem>();
  result->RegisterItem<PvAccessServerVariableItem>();

  // other items
  result->RegisterItem<InstructionContainerItem>();
  result->RegisterItem<JobItem>();
  result->RegisterItem<ProcedureItem>();
  result->RegisterItem<WorkspaceItem>();

  return result;
}

std::unique_ptr<mvvm::ItemManagerInterface> CreateSequencerItemManager(
    std::shared_ptr<mvvm::ItemPool> pool)
{
  return mvvm::CreateDefaultItemManager(CreateSequencerItemCatalogue(), std::move(pool));
}

}  // namespace sequencergui
