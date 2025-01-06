/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "sequencer_item_helper.h"

#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/sequencer_item_includes.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_catalogue.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/utils/container_utils.h>

#include <vector>

namespace
{

bool RegisterSequencerItems()
{
  // instructions
  (void)mvvm::RegisterGlobalItem<sequencergui::IncludeItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ParallelSequenceItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::RepeatItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::SequenceItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::WaitItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::UniversalInstructionItem>();

  // sequencer-plugin-epics instructions
  (void)mvvm::RegisterGlobalItem<sequencergui::ChannelAccessReadInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ChannelAccessWriteInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::PvAccessReadInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::PvAccessWriteInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::RPCClientInstruction>();
  (void)mvvm::RegisterGlobalItem<sequencergui::SystemCallInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::LogInstructionItem>();

  // variables
  (void)mvvm::RegisterGlobalItem<sequencergui::FileVariableItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::LocalVariableItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::UniversalVariableItem>();

  // variables sequencer-plugin-epics
  (void)mvvm::RegisterGlobalItem<sequencergui::ChannelAccessVariableItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::PvAccessClientVariableItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::PvAccessServerVariableItem>();

  // other items
  (void)mvvm::RegisterGlobalItem<sequencergui::InstructionContainerItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ProcedureItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::WorkspaceItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ProcedurePreambleItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::TypeRegistrationItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ProcedurePreambleItem>();

  (void)mvvm::RegisterGlobalItem<sequencergui::LocalJobItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ImportedJobItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::RemoteJobItem>();

  return true;
}

std::vector<std::string> GetDomainDecoratorNames()
{
  std::vector<std::string> result{sequencergui::domainconstants::kForceSuccessInstructionType,
                                  sequencergui::domainconstants::kIncludeInstructionType,
                                  sequencergui::domainconstants::kListenInstructionType,
                                  sequencergui::domainconstants::kRepeatInstructionType,
                                  sequencergui::domainconstants::kInverterInstructionType};
  return result;
}

mvvm::SessionItem *GetPropertyItem(const mvvm::SessionItem &parent, const std::string &tag)
{
  return mvvm::utils::HasTag(parent, tag) ? parent.GetItem(tag) : nullptr;
}

}  // namespace

namespace sequencergui
{

static bool sequencer_items_registered_flag = RegisterSequencerItems();

bool IsCompoundInstruction(const InstructionItem *instruction)
{
  return instruction->GetTaggedItems()->HasTag(itemconstants::kChildInstructions);
}

bool IsDecoratorInstruction(const InstructionItem *instruction)
{
  static const std::vector<std::string> domain_names = GetDomainDecoratorNames();
  return mvvm::utils::Contains(domain_names, instruction->GetDomainType());
}

mvvm::SessionItem *GetNameItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, itemconstants::kName);
}

mvvm::SessionItem *GetStatusItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, itemconstants::kStatus);
}

mvvm::SessionItem *GetBreakpointItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, itemconstants::kBreakpoint);
}

mvvm::SessionItem *GetIsAvailableItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, itemconstants::kIsAvailable);
}

mvvm::SessionItem *GetChannelItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, domainconstants::kChannelAttribute);
}

}  // namespace sequencergui
