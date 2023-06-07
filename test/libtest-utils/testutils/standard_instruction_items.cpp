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

// ----------------------------------------------------------------------------
// ForceSuccess
// ----------------------------------------------------------------------------

ForceSuccessItem::ForceSuccessItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> ForceSuccessItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ForceSuccessItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// IncrementItem
// ----------------------------------------------------------------------------

IncrementItem::IncrementItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> IncrementItem::Clone(bool make_unique_id) const
{
  return std::make_unique<IncrementItem>(*this, make_unique_id);
}

std::string IncrementItem::GetVariableName() const
{
  return Property<std::string>(sequencergui::domainconstants::kVarNameAttribute);
}

void IncrementItem::SetVariableName(const std::string &value)
{
  SetProperty(sequencergui::domainconstants::kVarNameAttribute, value);
}

// ----------------------------------------------------------------------------
// InputItem
// ----------------------------------------------------------------------------

InputItem::InputItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> InputItem::Clone(bool make_unique_id) const
{
  return std::make_unique<InputItem>(*this, make_unique_id);
}

std::string InputItem::GetTargetVariableName() const
{
  return Property<std::string>(sequencergui::domainconstants::kOutputAttribute);
}

void InputItem::SetTargetVariableName(const std::string &value)
{
  SetProperty(sequencergui::domainconstants::kOutputAttribute, value);
}

std::string InputItem::GetDescription() const
{
  return Property<std::string>(sequencergui::domainconstants::kDescriptionAttribute);
}

void InputItem::SetDescription(const std::string &value)
{
  SetProperty(sequencergui::domainconstants::kDescriptionAttribute, value);
}

// ----------------------------------------------------------------------------
// InverterItem
// ----------------------------------------------------------------------------

InverterItem::InverterItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> InverterItem::Clone(bool make_unique_id) const
{
  return std::make_unique<InverterItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// ListenItem
// ----------------------------------------------------------------------------

ListenItem::ListenItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> ListenItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ListenItem>(*this, make_unique_id);
}

bool ListenItem::IsForceSuccess() const
{
  return Property<bool>(sequencergui::domainconstants::kListenForceSuccessAttribute);
}

void ListenItem::SetForceSuccess(bool value)
{
  SetProperty(sequencergui::domainconstants::kListenForceSuccessAttribute, value);
}

std::string ListenItem::GetVarNames() const
{
  return Property<std::string>(sequencergui::domainconstants::kListenVarNamesAttribute);
}

void ListenItem::SetVarNames(const std::string &value)
{
  SetProperty(sequencergui::domainconstants::kListenVarNamesAttribute, value);
}

}  // namespace testutils
