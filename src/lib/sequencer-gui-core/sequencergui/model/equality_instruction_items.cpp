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

namespace sequencergui
{

// ----------------------------------------------------------------------------
// ComparisonItem
// ----------------------------------------------------------------------------

ComparisonItem::ComparisonItem(const std::string &item_type) : UniversalInstructionItem(item_type)
{
}

std::unique_ptr<mvvm::SessionItem> ComparisonItem::Clone(bool make_unique_id) const
{
  (void)make_unique_id;
  throw NotImplementedException("ComparisonItem::clone() should not be used");
}

std::string ComparisonItem::GetLeftHandSide() const
{
  return Property<std::string>(domainconstants::kLeftHandAttribute);
}

void ComparisonItem::SetLeftHandSide(const std::string &value)
{
  SetProperty(domainconstants::kLeftHandAttribute, value);
}

std::string ComparisonItem::GetRightHandSide() const
{
  return Property<std::string>(domainconstants::kRightHandAttribute);
}

void ComparisonItem::SetRightHandSide(const std::string &value)
{
  SetProperty(domainconstants::kRightHandAttribute, value);
}

// ----------------------------------------------------------------------------
// LessThanItem
// ----------------------------------------------------------------------------

LessThanItem::LessThanItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> LessThanItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LessThanItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// LessThanOrEqualItem
// ----------------------------------------------------------------------------

LessThanOrEqualItem::LessThanOrEqualItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> LessThanOrEqualItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LessThanOrEqualItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// EqualsItem
// ----------------------------------------------------------------------------

EqualsItem::EqualsItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> EqualsItem::Clone(bool make_unique_id) const
{
  return std::make_unique<EqualsItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// GreaterThanOrEqualItem
// ----------------------------------------------------------------------------

GreaterThanOrEqualItem::GreaterThanOrEqualItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> GreaterThanOrEqualItem::Clone(bool make_unique_id) const
{
  return std::make_unique<GreaterThanOrEqualItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// GreaterThanItem
// ----------------------------------------------------------------------------

GreaterThanItem::GreaterThanItem() : ComparisonItem(Type) {}

std::unique_ptr<mvvm::SessionItem> GreaterThanItem::Clone(bool make_unique_id) const
{
  return std::make_unique<GreaterThanItem>(*this, make_unique_id);
}

}  // namespace sequencergui
