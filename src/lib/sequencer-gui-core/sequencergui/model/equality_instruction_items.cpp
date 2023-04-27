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

#include <sequencergui/core/exceptions.h>
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
// ComparisonItem
// ----------------------------------------------------------------------------

ComparisonItem::ComparisonItem(const std::string &item_type) : InstructionItem(item_type)
{
  AddProperty(kLeftHandSide, std::string())->SetDisplayName("lhs");
  AddProperty(kRightHandSide, std::string())->SetDisplayName("rhs");
}

std::unique_ptr<mvvm::SessionItem> ComparisonItem::Clone(bool make_unique_id) const
{
  (void)make_unique_id;
  throw NotImplementedException("ComparisonItem::clone() should not be used");
}

std::string ComparisonItem::GetLeftHandSide() const
{
  return Property<std::string>(kLeftHandSide);
}

void ComparisonItem::SetLeftHandSide(const std::string &value)
{
  SetProperty(kLeftHandSide, value);
}

std::string ComparisonItem::GetRightHandSide() const
{
  return Property<std::string>(kRightHandSide);
}

void ComparisonItem::SetRightHandSide(const std::string &value)
{
  SetProperty(kRightHandSide, value);
}

void ComparisonItem::InitFromDomainImpl(const instruction_t *instruction)
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

void ComparisonItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kLeftHandAttribute, GetLeftHandSide(), *instruction);
  AddNonEmptyAttribute(domainconstants::kRightHandAttribute, GetRightHandSide(), *instruction);
}

// ----------------------------------------------------------------------------
// LessThanItem
// ----------------------------------------------------------------------------

LessThanItem::LessThanItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> LessThanItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LessThanItem>(*this, make_unique_id);
}

std::string LessThanItem::GetDomainType() const
{
  return domainconstants::kLessThanInstructionType;
}

// ----------------------------------------------------------------------------
// LessThanOrEqualItem
// ----------------------------------------------------------------------------

LessThanOrEqualItem::LessThanOrEqualItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> LessThanOrEqualItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LessThanOrEqualItem>(*this, make_unique_id);
}

std::string LessThanOrEqualItem::GetDomainType() const
{
  return domainconstants::kLessThanOrEqualInstructionType;
}

// ----------------------------------------------------------------------------
// EqualsItem
// ----------------------------------------------------------------------------

EqualsItem::EqualsItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> EqualsItem::Clone(bool make_unique_id) const
{
  return std::make_unique<EqualsItem>(*this, make_unique_id);
}

std::string EqualsItem::GetDomainType() const
{
  return domainconstants::kEqualsInstructionType;
}

// ----------------------------------------------------------------------------
// GreaterThanOrEqualItem
// ----------------------------------------------------------------------------

GreaterThanOrEqualItem::GreaterThanOrEqualItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> GreaterThanOrEqualItem::Clone(bool make_unique_id) const
{
  return std::make_unique<GreaterThanOrEqualItem>(*this, make_unique_id);
}

std::string GreaterThanOrEqualItem::GetDomainType() const
{
  return domainconstants::kGreaterThanOrEqualInstructionType;
}

// ----------------------------------------------------------------------------
// GreaterThanItem
// ----------------------------------------------------------------------------

GreaterThanItem::GreaterThanItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> GreaterThanItem::Clone(bool make_unique_id) const
{
  return std::make_unique<GreaterThanItem>(*this, make_unique_id);
}

std::string GreaterThanItem::GetDomainType() const
{
  return domainconstants::kGreaterThanInstructionType;
}

}  // namespace sequencergui
