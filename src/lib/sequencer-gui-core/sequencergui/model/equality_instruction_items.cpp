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

#include "equality_instruction_items.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/utils/string_utils.h>

#include <sup/sequencer/instruction.h>

namespace
{
const std::string kLeftHandSide = "kLeftHandSide";
const std::string kRightHandSide = "kRightHandSide";
}  // namespace

namespace sequencergui
{

// ----------------------------------------------------------------------------
// EqualsItem
// ----------------------------------------------------------------------------

EqualsItem::EqualsItem() : InstructionItem(Type)
{
  AddProperty(kLeftHandSide, std::string())->SetDisplayName("lhs");
  AddProperty(kRightHandSide, std::string())->SetDisplayName("rhs");
}

std::unique_ptr<mvvm::SessionItem> EqualsItem::Clone(bool make_unique_id) const
{
  return std::make_unique<EqualsItem>(*this, make_unique_id);
}

std::string EqualsItem::GetDomainType() const
{
  return domainconstants::kEqualsInstructionType;
}

std::string EqualsItem::GetLeftHandSide() const
{
  return Property<std::string>(kLeftHandSide);
}

void EqualsItem::SetLeftHandSide(const std::string &value)
{
  SetProperty(kLeftHandSide, value);
}

std::string EqualsItem::GetRightHandSide() const
{
  return Property<std::string>(kRightHandSide);
}

void EqualsItem::SetRightHandSide(const std::string &value)
{
  SetProperty(kRightHandSide, value);
}

void EqualsItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kLeftHandAttribute))
  {
    SetLeftHandSide(instruction->GetAttribute(domainconstants::kLeftHandAttribute));
  }
  if (instruction->HasAttribute(domainconstants::kRightHandAttribute))
  {
    SetRightHandSide(instruction->GetAttribute(domainconstants::kRightHandAttribute));
  }
}

void EqualsItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kLeftHandAttribute, GetLeftHandSide(), *instruction);
  AddNonEmptyAttribute(domainconstants::kRightHandAttribute, GetRightHandSide(), *instruction);
}

}  // namespace sequencergui
