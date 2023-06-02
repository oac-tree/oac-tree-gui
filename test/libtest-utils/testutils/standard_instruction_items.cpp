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

#include "standard_instruction_items.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>

namespace testutils
{

// ----------------------------------------------------------------------------
// ChoiceItem
// ----------------------------------------------------------------------------

ChoiceItem::ChoiceItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> ChoiceItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ChoiceItem>(*this, make_unique_id);
}

std::string ChoiceItem::GetVariableName() const
{
  return Property<std::string>(sequencergui::domainconstants::kVarNameAttribute);
}

void ChoiceItem::SetVariableName(const std::string &value)
{
  SetProperty(sequencergui::domainconstants::kVarNameAttribute, value);
}

// ----------------------------------------------------------------------------
// ConditionItem
// ----------------------------------------------------------------------------

ConditionItem::ConditionItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> ConditionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ConditionItem>(*this, make_unique_id);
}

std::string ConditionItem::GetVariableName() const
{
  return Property<std::string>(sequencergui::domainconstants::kVarNameAttribute);
}

void ConditionItem::SetVariableName(const std::string &value)
{
  SetProperty(sequencergui::domainconstants::kVarNameAttribute, value);
}

// ----------------------------------------------------------------------------
// CopyItem
// ----------------------------------------------------------------------------

CopyItem::CopyItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> CopyItem::Clone(bool make_unique_id) const
{
  return std::make_unique<CopyItem>(*this, make_unique_id);
}

std::string CopyItem::GetDomainType() const
{
  return sequencergui::domainconstants::kCopyInstructionType;
}

std::string CopyItem::GetInput() const
{
  return Property<std::string>(sequencergui::domainconstants::kInputAttribute);
}

void CopyItem::SetInput(const std::string &value)
{
  SetProperty(sequencergui::domainconstants::kInputAttribute, value);
}

std::string CopyItem::GetOutput() const
{
  return Property<std::string>(sequencergui::domainconstants::kOutputAttribute);
}

void CopyItem::SetOutput(const std::string &value)
{
  SetProperty(sequencergui::domainconstants::kOutputAttribute, value);
}

// ----------------------------------------------------------------------------
// DecrementItem
// ----------------------------------------------------------------------------

DecrementItem::DecrementItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> DecrementItem::Clone(bool make_unique_id) const
{
  return std::make_unique<DecrementItem>(*this, make_unique_id);
}

std::string DecrementItem::GetDomainType() const
{
  return sequencergui::domainconstants::kDecrementInstructionType;
}

std::string DecrementItem::GetVariableName() const
{
  return Property<std::string>(sequencergui::domainconstants::kVarNameAttribute);
}

void DecrementItem::SetVariableName(const std::string &value)
{
  SetProperty(sequencergui::domainconstants::kVarNameAttribute, value);
}

// ----------------------------------------------------------------------------
// FallbackItem
// ----------------------------------------------------------------------------
FallbackItem::FallbackItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> FallbackItem::Clone(bool make_unique_id) const
{
  return std::make_unique<FallbackItem>(*this, make_unique_id);
}

}  // namespace testutils
