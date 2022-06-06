/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/transformfromdomain.h"

#include "Instruction.h"
#include "Procedure.h"
#include "Workspace.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/guiobjectbuilder.h"
#include "sequencergui/model/instructioncontaineritem.h"
#include "sequencergui/model/standardinstructionitems.h"
#include "sequencergui/model/standardvariableitems.h"

#include "mvvm/model/function_types.h"
#include "mvvm/model/item_catalogue.h"

#include <stdexcept>

namespace
{
//! Creates catalogue where VariableItem factory functions are registered against "Type"
//! name strings known by the sequencer.
std::unique_ptr<mvvm::ItemCatalogue<sequencergui::VariableItem>> CreateVariableItemCatalogue()
{
  using mvvm::ItemFactoryFunction;
  auto result = std::make_unique<mvvm::ItemCatalogue<sequencergui::VariableItem>>();

  result->RegisterItem(sequencergui::domainconstants::kLocalVariableType,
                       ItemFactoryFunction<sequencergui::LocalVariableItem>);
  result->RegisterItem(sequencergui::domainconstants::kFileVariableType,
                       ItemFactoryFunction<sequencergui::FileVariableItem>);
  result->RegisterItem(sequencergui::domainconstants::kChannelAccessVariableType,
                       ItemFactoryFunction<sequencergui::ChannelAccessVariableItem>);
  result->RegisterItem(sequencergui::domainconstants::kPVClientVariableType,
                       ItemFactoryFunction<sequencergui::PVClientVariableItem>);

  return result;
}

//! Creates catalogue where InstructionItem factory functions are registered against "Type"
//! name strings known by the sequencer.
std::unique_ptr<mvvm::ItemCatalogue<sequencergui::InstructionItem>> CreateInstructionItemCatalogue()
{
  using mvvm::ItemFactoryFunction;
  auto result = std::make_unique<mvvm::ItemCatalogue<sequencergui::InstructionItem>>();

  result->RegisterItem(sequencergui::domainconstants::kConditionInstructionType,
                       ItemFactoryFunction<sequencergui::ConditionItem>);
  result->RegisterItem(sequencergui::domainconstants::kCopyInstructionType,
                       ItemFactoryFunction<sequencergui::CopyItem>);
  result->RegisterItem(sequencergui::domainconstants::kEqualsInstructionType,
                       ItemFactoryFunction<sequencergui::EqualsItem>);
  result->RegisterItem(sequencergui::domainconstants::kFallbackInstructionType,
                       ItemFactoryFunction<sequencergui::FallbackItem>);
  result->RegisterItem(sequencergui::domainconstants::kForceSuccessInstructionType,
                       ItemFactoryFunction<sequencergui::ForceSuccessItem>);
  result->RegisterItem(sequencergui::domainconstants::kIncludeInstructionType,
                       ItemFactoryFunction<sequencergui::IncludeItem>);
  result->RegisterItem(sequencergui::domainconstants::kInputInstructionType,
                       ItemFactoryFunction<sequencergui::InputItem>);
  result->RegisterItem(sequencergui::domainconstants::kInverterInstructionType,
                       ItemFactoryFunction<sequencergui::InverterItem>);
  result->RegisterItem(sequencergui::domainconstants::kListenInstructionType,
                       ItemFactoryFunction<sequencergui::ListenItem>);
  result->RegisterItem(sequencergui::domainconstants::kMessageInstructionType,
                       ItemFactoryFunction<sequencergui::MessageItem>);
  result->RegisterItem(sequencergui::domainconstants::kOutputInstructionType,
                       ItemFactoryFunction<sequencergui::OutputItem>);
  result->RegisterItem(sequencergui::domainconstants::kParallelInstructionType,
                       ItemFactoryFunction<sequencergui::ParallelSequenceItem>);
  result->RegisterItem(sequencergui::domainconstants::kRepeatInstructionType,
                       ItemFactoryFunction<sequencergui::RepeatItem>);
  result->RegisterItem(sequencergui::domainconstants::kSequenceInstructionType,
                       ItemFactoryFunction<sequencergui::SequenceItem>);
  result->RegisterItem(sequencergui::domainconstants::kUserChoiceInstructionType,
                       ItemFactoryFunction<sequencergui::UserChoiceItem>);
  result->RegisterItem(sequencergui::domainconstants::kWaitInstructionType,
                       ItemFactoryFunction<sequencergui::WaitItem>);

  return result;
}

}  // namespace

namespace sequencergui
{

std::string GetItemType(const std::string& domain_type)
{
  // For the moment it's one-to-one correspondance.
  return domain_type;
}

std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type)
{
  static const auto catalogue = CreateVariableItemCatalogue();

  if (!catalogue->Contains(domain_type))
  {
    return std::make_unique<UnknownVariableItem>();
  }

  return catalogue->Create(domain_type);
}

std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type)
{
  static const auto catalogue = CreateInstructionItemCatalogue();

  if (!catalogue->Contains(domain_type))
  {
    return std::make_unique<UnknownInstructionItem>();
  }

  return catalogue->Create(domain_type);
}

void PopulateWorkspaceItem(const procedure_t* procedure, WorkspaceItem* workspace)
{
  GUIObjectBuilder builder;
  builder.PopulateWorkspaceItem(procedure, workspace);
}

void PopulateInstructionContainerItem(const procedure_t* procedure,
                                      InstructionContainerItem* container)
{
  GUIObjectBuilder builder;
  builder.PopulateInstructionContainerItem(procedure, container);
}

std::unique_ptr<InstructionItem> CreateUnknownInstructionItem(const std::string& domain_type)
{
  // This handles a special case when instruction's domain_type is unknown to the GUI.
  // We create UnkownInstructionItem and initialise it's attributes from the temporary domain
  // instruction.
  auto domain_instruction = DomainUtils::CreateDomainInstruction(domain_type);
  auto result = std::make_unique<UnknownInstructionItem>();
  result->InitFromDomain(domain_instruction.get());
  return result;
}

}  // namespace sequencergui
