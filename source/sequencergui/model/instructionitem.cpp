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

#include "sequencergui/model/instructionitem.h"

#include "Instruction.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/sequenceritems.h"

#include "mvvm/model/taggeditems.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/utils/stringutils.h"

namespace sequi
{
// ----------------------------------------------------------------------------
// InstructionItem
// ----------------------------------------------------------------------------
static inline const std::string kName = "kName";
static inline const std::string kStatus = "kStatus";
static inline const std::string kDescription = "kDescription";
static inline const std::string kXpos = "kXpos";
static inline const std::string kYpos = "kYpos";
static inline const std::string kChildInstructions = "kChildInstructions";
static inline const std::string kIsRoot = "kIsRoot";

InstructionItem::InstructionItem(const std::string &item_type) : CompoundItem(item_type)
{
  AddProperty(kName, std::string())->SetDisplayName("Name");
  AddProperty(kIsRoot, false)->SetDisplayName("IsRoot");
  AddProperty(kStatus, std::string())->SetDisplayName("Status");
  AddProperty(kXpos, 0.0)->SetDisplayName("X");
  AddProperty(kYpos, 0.0)->SetDisplayName("Y");
}

void InstructionItem::InitFromDomain(const instruction_t *instruction)
{
  if (instruction->GetType() != GetDomainType() && GetType() != UnknownInstructionItem::Type)
  {
    throw std::runtime_error("Error in InstructionItem: domain instruction doesn't match.");
  }

  if (instruction->HasAttribute(DomainConstants::kNameAttribute))
  {
    SetProperty(kName, instruction->GetAttribute(DomainConstants::kNameAttribute));
  }

  SetIsRootFlag(DomainUtils::IsRootInstruction(instruction));

  InitFromDomainImpl(instruction);
}

std::unique_ptr<instruction_t> InstructionItem::CreateDomainInstruction() const
{
  auto result = DomainUtils::CreateDomainInstruction(GetDomainType());

  result->AddAttribute(DomainConstants::kNameAttribute, Property<std::string>(kName));
  result->AddAttribute(DomainConstants::kIsRootAttribute,
                       ModelView::Utils::FromBool(Property<bool>(kIsRoot)));
  SetupDomainImpl(result.get());
  return result;
}

//! Returns all children instruction, if exist.

std::vector<InstructionItem *> InstructionItem::GetInstructions() const
{
  return GetTaggedItems()->HasTag(kChildInstructions)
             ? GetItems<InstructionItem>(kChildInstructions)
             : std::vector<InstructionItem *>();
}

bool InstructionItem::IsRoot() const
{
  return Property<bool>(kIsRoot);
}

void InstructionItem::SetIsRootFlag(bool value)
{
  SetProperty(kIsRoot, value);
}

void InstructionItem::SetStatus(const std::string &status)
{
  SetProperty(kStatus, status);
}

std::string InstructionItem::GetStatus() const
{
  return Property<std::string>(kStatus);
}

double InstructionItem::GetX() const
{
  return Property<double>(kXpos);
}

void InstructionItem::SetX(double value)
{
  SetProperty(kXpos, value);
}

double InstructionItem::GetY() const
{
  return Property<double>(kYpos);
}

void InstructionItem::SetY(double value)
{
  SetProperty(kYpos, value);
}

ModelView::SessionItem *InstructionItem::GetStatusItem() const
{
  return GetItem(kStatus);
}

ModelView::SessionItem *InstructionItem::GetNameItem() const
{
  return GetItem(kName);
}

}  // namespace sequi
