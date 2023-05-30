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

namespace sequencergui
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
  return Property<std::string>(domainconstants::kVarNameAttribute);
}

void ChoiceItem::SetVariableName(const std::string &value)
{
  SetProperty(domainconstants::kVarNameAttribute, value);
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
  return Property<std::string>(domainconstants::kVarNameAttribute);
}

void ConditionItem::SetVariableName(const std::string &value)
{
  SetProperty(domainconstants::kVarNameAttribute, value);
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
  return domainconstants::kCopyInstructionType;
}

std::string CopyItem::GetInput() const
{
  return Property<std::string>(domainconstants::kInputAttribute);
}

void CopyItem::SetInput(const std::string &value)
{
  SetProperty(domainconstants::kInputAttribute, value);
}

std::string CopyItem::GetOutput() const
{
  return Property<std::string>(domainconstants::kOutputAttribute);
}

void CopyItem::SetOutput(const std::string &value)
{
  SetProperty(domainconstants::kOutputAttribute, value);
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
  return domainconstants::kDecrementInstructionType;
}

std::string DecrementItem::GetVariableName() const
{
  return Property<std::string>(domainconstants::kVarNameAttribute);
}

void DecrementItem::SetVariableName(const std::string &value)
{
  SetProperty(domainconstants::kVarNameAttribute, value);
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
// IncludeItem
// ----------------------------------------------------------------------------

IncludeItem::IncludeItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> IncludeItem::Clone(bool make_unique_id) const
{
  return std::make_unique<IncludeItem>(*this, make_unique_id);
}

std::string IncludeItem::GetFileName() const
{
  return Property<std::string>(domainconstants::kFileAttribute);
}

void IncludeItem::SetFileName(const std::string &value)
{
  SetProperty(domainconstants::kFileAttribute, value);
}

std::string IncludeItem::GetPath() const
{
  return Property<std::string>(domainconstants::kPathAttribute);
}

void IncludeItem::SetPath(const std::string &value)
{
  SetProperty(domainconstants::kPathAttribute, value);
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
  return Property<std::string>(domainconstants::kVarNameAttribute);
}

void IncrementItem::SetVariableName(const std::string &value)
{
  SetProperty(domainconstants::kVarNameAttribute, value);
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
  return Property<std::string>(domainconstants::kInputTargetAttribute);
}

void InputItem::SetTargetVariableName(const std::string &value)
{
  SetProperty(domainconstants::kInputTargetAttribute, value);
}

std::string InputItem::GetDescription() const
{
  return Property<std::string>(domainconstants::kDescriptionAttribute);
}

void InputItem::SetDescription(const std::string &value)
{
  SetProperty(domainconstants::kDescriptionAttribute, value);
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
  return Property<bool>(domainconstants::kListenForceSuccessAttribute);
}

void ListenItem::SetForceSuccess(bool value)
{
  SetProperty(domainconstants::kListenForceSuccessAttribute, value);
}

std::string ListenItem::GetVarNames() const
{
  return Property<std::string>(domainconstants::kListenVarNamesAttribute);
}

void ListenItem::SetVarNames(const std::string &value)
{
  SetProperty(domainconstants::kListenVarNamesAttribute, value);
}

// ----------------------------------------------------------------------------
// MessageItem
// ----------------------------------------------------------------------------

MessageItem::MessageItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> MessageItem::Clone(bool make_unique_id) const
{
  return std::make_unique<MessageItem>(*this, make_unique_id);
}

std::string MessageItem::GetText() const
{
  return Property<std::string>(domainconstants::kTextAttribute);
}

void MessageItem::SetText(const std::string &value)
{
  SetProperty(domainconstants::kTextAttribute, value);
}

// ----------------------------------------------------------------------------
// OutputItem
// ----------------------------------------------------------------------------

OutputItem::OutputItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> OutputItem::Clone(bool make_unique_id) const
{
  return std::make_unique<OutputItem>(*this, make_unique_id);
}

std::string OutputItem::GetSourceVariableName() const
{
  return Property<std::string>(domainconstants::kOutputSourceAttribute);
}

void OutputItem::SetSourceVariableName(const std::string &value)
{
  SetProperty(domainconstants::kOutputSourceAttribute, value);
}

std::string OutputItem::GetDescription() const
{
  return Property<std::string>(domainconstants::kDescriptionAttribute);
}

void OutputItem::SetDescription(const std::string &value)
{
  SetProperty(domainconstants::kDescriptionAttribute, value);
}

// ----------------------------------------------------------------------------
// ParallelSequenceItem
// ----------------------------------------------------------------------------

ParallelSequenceItem::ParallelSequenceItem() : UniversalInstructionItem(Type)
{
  // FIXME How to define default values? Or, how to pretend that attribute wasn't set?
  SetSuccessThreshold(0);
  SetFailureThreshold(1);
}

std::unique_ptr<mvvm::SessionItem> ParallelSequenceItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ParallelSequenceItem>(*this, make_unique_id);
}

int ParallelSequenceItem::GetSuccessThreshold() const
{
  return Property<int>(domainconstants::kSuccessThresholdAttribute);
}

void ParallelSequenceItem::SetSuccessThreshold(int value)
{
  SetProperty(domainconstants::kSuccessThresholdAttribute, value);
}

int ParallelSequenceItem::GetFailureThreshold() const
{
  return Property<int>(domainconstants::kFailureThresholdAttribute);
}

void ParallelSequenceItem::SetFailureThreshold(int value)
{
  SetProperty(domainconstants::kFailureThresholdAttribute, value);
}

// ----------------------------------------------------------------------------
// RepeatItem
// ----------------------------------------------------------------------------

RepeatItem::RepeatItem() : UniversalInstructionItem(Type)
{
  SetRepeatCount(-1);
}

std::unique_ptr<mvvm::SessionItem> RepeatItem::Clone(bool make_unique_id) const
{
  return std::make_unique<RepeatItem>(*this, make_unique_id);
}

int RepeatItem::GetRepeatCount() const
{
  return Property<int>(domainconstants::kMaxCountAttribute);
}

void RepeatItem::SetRepeatCount(int value)
{
  SetProperty(domainconstants::kMaxCountAttribute, value);
}

// ----------------------------------------------------------------------------
// SequenceItem
// ----------------------------------------------------------------------------
SequenceItem::SequenceItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> SequenceItem::Clone(bool make_unique_id) const
{
  return std::make_unique<SequenceItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// UserChoiceItem
// ----------------------------------------------------------------------------

UserChoiceItem::UserChoiceItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> UserChoiceItem::Clone(bool make_unique_id) const
{
  return std::make_unique<UserChoiceItem>(*this, make_unique_id);
}

std::string UserChoiceItem::GetDescription() const
{
  return Property<std::string>(domainconstants::kDescriptionAttribute);
}

void UserChoiceItem::SetDescription(const std::string &value)
{
  SetProperty(domainconstants::kDescriptionAttribute, value);
}

// ----------------------------------------------------------------------------
// VariableResetItem
// ----------------------------------------------------------------------------

VariableResetItem::VariableResetItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> VariableResetItem::Clone(bool make_unique_id) const
{
  return std::make_unique<VariableResetItem>(*this, make_unique_id);
}

std::string VariableResetItem::GetVariableName() const
{
  return Property<std::string>(domainconstants::kVarNameAttribute);
}

void VariableResetItem::SetVariableName(const std::string &value)
{
  SetProperty(domainconstants::kVarNameAttribute, value);
}

// ----------------------------------------------------------------------------
// WaitItem
// ----------------------------------------------------------------------------

WaitItem::WaitItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> WaitItem::Clone(bool make_unique_id) const
{
  return std::make_unique<WaitItem>(*this, make_unique_id);
}

void WaitItem::SetTimeout(double value)
{
  SetProperty(domainconstants::kTimeoutAttribute, value);
}

double WaitItem::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

}  // namespace sequencergui
