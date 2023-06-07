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

#include "transform_from_domain.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/sequencer_item_includes.h>
#include <sequencergui/transform/gui_object_builder.h>

#include <mvvm/model/function_types.h>
#include <mvvm/model/item_catalogue.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

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
  result->RegisterItem(sequencergui::domainconstants::kPvAccessClientVariableType,
                       ItemFactoryFunction<sequencergui::PvAccessClientVariableItem>);
  result->RegisterItem(sequencergui::domainconstants::kPvAccessServerVariableType,
                       ItemFactoryFunction<sequencergui::PvAccessServerVariableItem>);

  return result;
}

//! Creates catalogue where InstructionItem factory functions are registered against "Type"
//! name strings known by the sequencer.
std::unique_ptr<mvvm::ItemCatalogue<sequencergui::InstructionItem>> CreateInstructionItemCatalogue()
{
  using namespace sequencergui::domainconstants;
  using mvvm::ItemFactoryFunction;
  auto result = std::make_unique<mvvm::ItemCatalogue<sequencergui::InstructionItem>>();

  result->RegisterItem(kIncludeInstructionType, ItemFactoryFunction<sequencergui::IncludeItem>);
  result->RegisterItem(kOutputInstructionType, ItemFactoryFunction<sequencergui::OutputItem>);
  result->RegisterItem(kParallelInstructionType,
                       ItemFactoryFunction<sequencergui::ParallelSequenceItem>);
  result->RegisterItem(kRepeatInstructionType, ItemFactoryFunction<sequencergui::RepeatItem>);
  result->RegisterItem(kSequenceInstructionType, ItemFactoryFunction<sequencergui::SequenceItem>);
  result->RegisterItem(kUserChoiceInstructionType,
                       ItemFactoryFunction<sequencergui::UserChoiceItem>);
  result->RegisterItem(kVariableResetInstructionType,
                       ItemFactoryFunction<sequencergui::VariableResetItem>);
  result->RegisterItem(kWaitInstructionType, ItemFactoryFunction<sequencergui::WaitItem>);

  // instructions from sequencer-plugin-epics
  result->RegisterItem(kChannelAccessReadInstructionType,
                       ItemFactoryFunction<sequencergui::ChannelAccessReadInstructionItem>);
  result->RegisterItem(kChannelAccessWriteInstructionType,
                       ItemFactoryFunction<sequencergui::ChannelAccessWriteInstructionItem>);
  result->RegisterItem(kPvAccessReadInstructionType,
                       ItemFactoryFunction<sequencergui::PvAccessReadInstructionItem>);
  result->RegisterItem(kPvAccessWriteInstructionType,
                       ItemFactoryFunction<sequencergui::PvAccessWriteInstructionItem>);
  result->RegisterItem(kRPCClientInstructionType,
                       ItemFactoryFunction<sequencergui::RPCClientInstruction>);
  result->RegisterItem(kSystemCallInstructionType,
                       ItemFactoryFunction<sequencergui::SystemCallInstructionItem>);
  result->RegisterItem(kLogInstructionType, ItemFactoryFunction<sequencergui::LogInstructionItem>);

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
    auto result = std::make_unique<UniversalVariableItem>();
    result->SetDomainType(domain_type);
    return result;
  }

  return catalogue->Create(domain_type);
}

std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type)
{
  static const auto catalogue = CreateInstructionItemCatalogue();

  if (!catalogue->Contains(domain_type))
  {
    auto result = std::make_unique<UniversalInstructionItem>();
    result->SetDomainType(domain_type);
    return result;
  }

  return catalogue->Create(domain_type);
}

std::unique_ptr<ProcedureItem> CreateProcedureItem(const procedure_t* procedure, bool root_only)
{
  GUIObjectBuilder builder;
  return builder.CreateProcedureItem(procedure, root_only);
}

}  // namespace sequencergui
