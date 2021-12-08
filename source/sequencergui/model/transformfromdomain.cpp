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

  result->RegisterItem(sequi::DomainConstants::kLocalVariableType,
                       ItemFactoryFunction<sequi::LocalVariableItem>());
  result->RegisterItem(sequi::DomainConstants::kFileVariableType,
                       ItemFactoryFunction<sequi::FileVariableItem>());
  result->RegisterItem(sequi::DomainConstants::kChannelAccessVariableType,
                       ItemFactoryFunction<sequi::ChannelAccessVariableItem>());
  result->RegisterItem(sequi::DomainConstants::kPVClientVariableType,
                       ItemFactoryFunction<sequi::PVClientVariableItem>());

  return result;
}

//! Creates catalogue where InstructionItem factory functions are registered against "Type"
//! name strings known by the sequencer.
std::unique_ptr<ModelView::ItemCatalogue> CreateInstructionItemCatalogue()
{
  using ModelView::ItemFactoryFunction;
  auto result = std::make_unique<ModelView::ItemCatalogue>();

  result->RegisterItem(sequi::DomainConstants::kConditionInstructionType,
                       ItemFactoryFunction<sequi::ConditionItem>());
  result->RegisterItem(sequi::DomainConstants::kCopyInstructionType,
                       ItemFactoryFunction<sequi::CopyItem>());
  result->RegisterItem(sequi::DomainConstants::kFallbackInstructionType,
                       ItemFactoryFunction<sequi::FallbackItem>());
  result->RegisterItem(sequi::DomainConstants::kIncludeInstructionType,
                       ItemFactoryFunction<sequi::IncludeItem>());
  result->RegisterItem(sequi::DomainConstants::kInputInstructionType,
                       ItemFactoryFunction<sequi::InputItem>());
  result->RegisterItem(sequi::DomainConstants::kInverterInstructionType,
                       ItemFactoryFunction<sequi::InverterItem>());
  result->RegisterItem(sequi::DomainConstants::kOutputInstructionType,
                       ItemFactoryFunction<sequi::OutputItem>());
  result->RegisterItem(sequi::DomainConstants::kRepeatInstructionType,
                       ItemFactoryFunction<sequi::RepeatItem>());
  result->RegisterItem(sequi::DomainConstants::kSequenceInstructionType,
                       ItemFactoryFunction<sequi::SequenceItem>());
  result->RegisterItem(sequi::DomainConstants::kWaitInstructionType,
                       ItemFactoryFunction<sequi::WaitItem>());
  result->RegisterItem(sequi::DomainConstants::kParallelInstructionType,
                       ItemFactoryFunction<sequi::ParallelSequenceItem>());
  result->RegisterItem(sequi::DomainConstants::kMessageInstructionType,
                       ItemFactoryFunction<sequi::MessageItem>());
  result->RegisterItem(sequi::DomainConstants::kEqualsInstructionType,
                       ItemFactoryFunction<sequi::EqualsItem>());
  result->RegisterItem(sequi::DomainConstants::kForceSuccessInstructionType,
                       ItemFactoryFunction<sequi::ForceSuccessItem>());
  result->RegisterItem(sequi::DomainConstants::kUserChoiceInstructionType,
                       ItemFactoryFunction<sequi::UserChoiceItem>());

  return result;
}

}  // namespace

namespace sequi
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
