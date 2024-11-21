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
  mvvm::RegisterGlobalItem<sequencergui::IncludeItem>();
  mvvm::RegisterGlobalItem<sequencergui::ParallelSequenceItem>();
  mvvm::RegisterGlobalItem<sequencergui::RepeatItem>();
  mvvm::RegisterGlobalItem<sequencergui::SequenceItem>();
  mvvm::RegisterGlobalItem<sequencergui::WaitItem>();
  mvvm::RegisterGlobalItem<sequencergui::UniversalInstructionItem>();

  // sequencer-plugin-epics instructions
  mvvm::RegisterGlobalItem<sequencergui::ChannelAccessReadInstructionItem>();
  mvvm::RegisterGlobalItem<sequencergui::ChannelAccessWriteInstructionItem>();
  mvvm::RegisterGlobalItem<sequencergui::PvAccessReadInstructionItem>();
  mvvm::RegisterGlobalItem<sequencergui::PvAccessWriteInstructionItem>();
  mvvm::RegisterGlobalItem<sequencergui::RPCClientInstruction>();
  mvvm::RegisterGlobalItem<sequencergui::SystemCallInstructionItem>();
  mvvm::RegisterGlobalItem<sequencergui::LogInstructionItem>();

  // variables
  mvvm::RegisterGlobalItem<sequencergui::FileVariableItem>();
  mvvm::RegisterGlobalItem<sequencergui::LocalVariableItem>();
  mvvm::RegisterGlobalItem<sequencergui::UniversalVariableItem>();

  // variables sequencer-plugin-epics
  mvvm::RegisterGlobalItem<sequencergui::ChannelAccessVariableItem>();
  mvvm::RegisterGlobalItem<sequencergui::PvAccessClientVariableItem>();
  mvvm::RegisterGlobalItem<sequencergui::PvAccessServerVariableItem>();

  // other items
  mvvm::RegisterGlobalItem<sequencergui::InstructionContainerItem>();
  mvvm::RegisterGlobalItem<sequencergui::ProcedureItem>();
  mvvm::RegisterGlobalItem<sequencergui::WorkspaceItem>();
  mvvm::RegisterGlobalItem<sequencergui::ProcedurePreambleItem>();
  mvvm::RegisterGlobalItem<sequencergui::TypeRegistrationItem>();
  mvvm::RegisterGlobalItem<sequencergui::ProcedurePreambleItem>();

  mvvm::RegisterGlobalItem<sequencergui::LocalJobItem>();
  mvvm::RegisterGlobalItem<sequencergui::ImportedJobItem>();
  mvvm::RegisterGlobalItem<sequencergui::RemoteJobItem>();

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
