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
#include <sequencergui/transform/gui_object_builder.h>

#include <mvvm/model/function_types.h>
#include <mvvm/model/item_catalogue.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/procedure_preamble.h>
#include <sup/sequencer/workspace.h>

namespace
{

/**
 * @brief Creates catalogue where VariableItem factory functions are registered against "Type" name
 * strings known by the sequencer.
 * @return
 */
std::unique_ptr<mvvm::ItemCatalogue<sequencergui::VariableItem>> CreateVariableItemCatalogue()
{
  auto result = std::make_unique<mvvm::ItemCatalogue<sequencergui::VariableItem>>();

  result->RegisterItem<sequencergui::LocalVariableItem>();
  result->RegisterItem<sequencergui::FileVariableItem>();
  result->RegisterItem<sequencergui::ChannelAccessVariableItem>();
  result->RegisterItem<sequencergui::PvAccessClientVariableItem>();
  result->RegisterItem<sequencergui::PvAccessServerVariableItem>();

  return result;
}

/**
 * @brief Creates catalogue where InstructionItem factory functions are registered against "Type"
 * name strings known by the sequencer.
 */
std::unique_ptr<mvvm::ItemCatalogue<sequencergui::InstructionItem>> CreateInstructionItemCatalogue()
{
  auto result = std::make_unique<mvvm::ItemCatalogue<sequencergui::InstructionItem>>();

  result->RegisterItem<sequencergui::IncludeItem>();
  result->RegisterItem<sequencergui::ParallelSequenceItem>();
  result->RegisterItem<sequencergui::RepeatItem>();
  result->RegisterItem<sequencergui::SequenceItem>();
  result->RegisterItem<sequencergui::WaitItem>();

  // instructions from sequencer-plugin-epics
  result->RegisterItem<sequencergui::ChannelAccessReadInstructionItem>();
  result->RegisterItem<sequencergui::ChannelAccessWriteInstructionItem>();
  result->RegisterItem<sequencergui::PvAccessReadInstructionItem>();
  result->RegisterItem<sequencergui::PvAccessWriteInstructionItem>();
  result->RegisterItem<sequencergui::RPCClientInstruction>();
  result->RegisterItem<sequencergui::SystemCallInstructionItem>();
  result->RegisterItem<sequencergui::LogInstructionItem>();

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

std::unique_ptr<ProcedureItem> CreateProcedureItem(const procedure_t* procedure, bool root_only)
{
  GUIObjectBuilder builder;
  return builder.CreateProcedureItem(procedure, root_only);
}

void PopulateProcedurePreambleItem(const preamble_t& preamble, ProcedurePreambleItem& item)
{
  if (!item.GetPluginPaths().empty() || !item.GetTypeRegistrations().empty())
  {
    throw LogicErrorException("ProcedurePreambleItem must be empty");
  }

  for (const auto& str : preamble.GetPluginPaths())
  {
    item.AddPluginPath(str);
  }

  for (const auto& info : preamble.GetTypeRegistrations())
  {
    item.AddTypeRegistration(static_cast<int>(info.GetRegistrationMode()), info.GetString());
  }
}

}  // namespace sequencergui
