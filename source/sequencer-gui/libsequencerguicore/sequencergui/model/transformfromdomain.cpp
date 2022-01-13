/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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
#include "sequencergui/model/guiobjectbuilder.h"
#include "sequencergui/model/sequenceritems.h"

#include "mvvm/model/itemcatalogue.h"

#include <stdexcept>

namespace
{
//! Creates catalogue where VariableItem factory functions are registered against "Type"
//! name strings known by the sequencer.
std::unique_ptr<ModelView::ItemCatalogue> CreateVariableItemCatalogue()
{
  using ModelView::ItemFactoryFunction;
  auto result = std::make_unique<ModelView::ItemCatalogue>();

  result->RegisterItem(sequencergui::DomainConstants::kLocalVariableType,
                       ItemFactoryFunction<sequencergui::LocalVariableItem>());
  result->RegisterItem(sequencergui::DomainConstants::kFileVariableType,
                       ItemFactoryFunction<sequencergui::FileVariableItem>());
  result->RegisterItem(sequencergui::DomainConstants::kChannelAccessVariableType,
                       ItemFactoryFunction<sequencergui::ChannelAccessVariableItem>());
  result->RegisterItem(sequencergui::DomainConstants::kPVClientVariableType,
                       ItemFactoryFunction<sequencergui::PVClientVariableItem>());

  return result;
}

//! Creates catalogue where InstructionItem factory functions are registered against "Type"
//! name strings known by the sequencer.
std::unique_ptr<ModelView::ItemCatalogue> CreateInstructionItemCatalogue()
{
  using ModelView::ItemFactoryFunction;
  auto result = std::make_unique<ModelView::ItemCatalogue>();

  result->RegisterItem(sequencergui::DomainConstants::kConditionInstructionType,
                       ItemFactoryFunction<sequencergui::ConditionItem>());
  result->RegisterItem(sequencergui::DomainConstants::kCopyInstructionType,
                       ItemFactoryFunction<sequencergui::CopyItem>());
  result->RegisterItem(sequencergui::DomainConstants::kFallbackInstructionType,
                       ItemFactoryFunction<sequencergui::FallbackItem>());
  result->RegisterItem(sequencergui::DomainConstants::kIncludeInstructionType,
                       ItemFactoryFunction<sequencergui::IncludeItem>());
  result->RegisterItem(sequencergui::DomainConstants::kInputInstructionType,
                       ItemFactoryFunction<sequencergui::InputItem>());
  result->RegisterItem(sequencergui::DomainConstants::kInverterInstructionType,
                       ItemFactoryFunction<sequencergui::InverterItem>());
  result->RegisterItem(sequencergui::DomainConstants::kOutputInstructionType,
                       ItemFactoryFunction<sequencergui::OutputItem>());
  result->RegisterItem(sequencergui::DomainConstants::kRepeatInstructionType,
                       ItemFactoryFunction<sequencergui::RepeatItem>());
  result->RegisterItem(sequencergui::DomainConstants::kSequenceInstructionType,
                       ItemFactoryFunction<sequencergui::SequenceItem>());
  result->RegisterItem(sequencergui::DomainConstants::kWaitInstructionType,
                       ItemFactoryFunction<sequencergui::WaitItem>());
  result->RegisterItem(sequencergui::DomainConstants::kParallelInstructionType,
                       ItemFactoryFunction<sequencergui::ParallelSequenceItem>());
  result->RegisterItem(sequencergui::DomainConstants::kMessageInstructionType,
                       ItemFactoryFunction<sequencergui::MessageItem>());
  result->RegisterItem(sequencergui::DomainConstants::kEqualsInstructionType,
                       ItemFactoryFunction<sequencergui::EqualsItem>());
  result->RegisterItem(sequencergui::DomainConstants::kForceSuccessInstructionType,
                       ItemFactoryFunction<sequencergui::ForceSuccessItem>());
  result->RegisterItem(sequencergui::DomainConstants::kUserChoiceInstructionType,
                       ItemFactoryFunction<sequencergui::UserChoiceItem>());

  return result;
}

}  // namespace

namespace sequencergui
{
std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type)
{
  static const auto catalogue = CreateVariableItemCatalogue();

  if (!catalogue->Contains(domain_type))
  {
    return std::make_unique<UnknownVariableItem>();
  }

  // FIXME remove downcast and unique_ptr recreation
  return std::unique_ptr<VariableItem>(
      static_cast<VariableItem*>(catalogue->Create(domain_type).release()));
}

std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type)
{
  static const auto catalogue = CreateInstructionItemCatalogue();

  if (!catalogue->Contains(domain_type))
  {
    return std::make_unique<UnknownInstructionItem>();
  }

  // FIXME remove downcast and unique_ptr recreation
  return std::unique_ptr<InstructionItem>(
      static_cast<InstructionItem*>(catalogue->Create(domain_type).release()));
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

}  // namespace sequi
