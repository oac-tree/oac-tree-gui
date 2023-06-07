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

#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/sequencer_item_includes.h>

#include <mvvm/model/item_catalogue.h>
#include <mvvm/model/item_manager.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/utils/container_utils.h>

#include <sup/gui/model/anyvalue_item.h>

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
  result->RegisterItem<IncludeItem>();
  result->RegisterItem<ParallelSequenceItem>();
  result->RegisterItem<RepeatItem>();
  result->RegisterItem<SequenceItem>();
  result->RegisterItem<VariableResetItem>();
  result->RegisterItem<WaitItem>();
  result->RegisterItem<UniversalInstructionItem>();

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

  result->RegisterItem<sup::gui::AnyValueEmptyItem>();
  result->RegisterItem<sup::gui::AnyValueScalarItem>();
  result->RegisterItem<sup::gui::AnyValueStructItem>();
  result->RegisterItem<sup::gui::AnyValueArrayItem>();

  return result;
}

std::unique_ptr<mvvm::ItemManagerInterface> CreateSequencerItemManager(
    std::shared_ptr<mvvm::ItemPool> pool)
{
  return mvvm::CreateDefaultItemManager(CreateSequencerItemCatalogue(), std::move(pool));
}

}  // namespace sequencergui
