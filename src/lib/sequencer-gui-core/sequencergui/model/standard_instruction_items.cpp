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
#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/utils/string_utils.h>

#include <sup/sequencer/instruction.h>

namespace
{
const std::string kVariableName = "varName";
const std::string kTarget = "kTarget";
const std::string kForceSuccess = "kForceSuccess";
const std::string kVarNames = "kVarNames";
const std::string kText = "kText";
const std::string kSuccessThreshold = "kSuccessThreshold";
const std::string kFailureThreshold = "kFailureThreshold";
const std::string kSource = "kSource";
const std::string kMaxCount = "kRepeatCount";
const std::string kTimeout = "timeout";
}  // namespace

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

InputItem::InputItem() : UniversalInstructionItem(Type)
{
}

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

InverterItem::InverterItem() : InstructionItem(Type)
{
  RegisterCommonProperties();
  RegisterTag(mvvm::TagInfo(itemconstants::kChildInstructions, 0, 1, {}), /*as_default*/ true);
}

std::unique_ptr<mvvm::SessionItem> InverterItem::Clone(bool make_unique_id) const
{
  return std::make_unique<InverterItem>(*this, make_unique_id);
}

std::string InverterItem::GetDomainType() const
{
  return domainconstants::kInverterInstructionType;
}

void InverterItem::InitFromDomainImpl(const instruction_t *instruction)
{
  (void)instruction;
}

void InverterItem::SetupDomainImpl(instruction_t *instruction) const
{
  (void)instruction;
}

// ----------------------------------------------------------------------------
// ListenItem
// ----------------------------------------------------------------------------

ListenItem::ListenItem() : InstructionItem(Type)
{
  RegisterCommonProperties();
  AddProperty(kForceSuccess, false)->SetDisplayName("Force success");
  AddProperty(kVarNames, std::string())->SetDisplayName("Var names");
  RegisterTag(mvvm::TagInfo(itemconstants::kChildInstructions, 0, 1, {}), /*as_default*/ true);
}

std::unique_ptr<mvvm::SessionItem> ListenItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ListenItem>(*this, make_unique_id);
}

std::string ListenItem::GetDomainType() const
{
  return domainconstants::kListenInstructionType;
}

bool ListenItem::IsForceSuccess() const
{
  return Property<bool>(kForceSuccess);
}

void ListenItem::SetForceSuccess(bool value)
{
  SetProperty(kForceSuccess, value);
}

std::string ListenItem::GetVarNames() const
{
  return Property<std::string>(kVarNames);
}

void ListenItem::SetVarNames(const std::string &value)
{
  SetProperty(kVarNames, value);
}

void ListenItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kListenForceSuccessAttribute))
  {
    SetForceSuccess(::mvvm::utils::StringToBool(
        instruction->GetAttributeString(domainconstants::kListenForceSuccessAttribute)));
  }

  if (instruction->HasAttribute(domainconstants::kListenVarNamesAttribute))
  {
    SetVarNames(instruction->GetAttributeString(domainconstants::kListenVarNamesAttribute));
  }
}

void ListenItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kListenForceSuccessAttribute,
                       mvvm::utils::FromBool(IsForceSuccess()), *instruction);
  AddNonEmptyAttribute(domainconstants::kListenVarNamesAttribute, GetVarNames(), *instruction);
}

// ----------------------------------------------------------------------------
// MessageItem
// ----------------------------------------------------------------------------

MessageItem::MessageItem() : InstructionItem(Type)
{
  RegisterCommonProperties();
  AddProperty(kText, std::string())->SetDisplayName("text");
}

std::unique_ptr<mvvm::SessionItem> MessageItem::Clone(bool make_unique_id) const
{
  return std::make_unique<MessageItem>(*this, make_unique_id);
}

std::string MessageItem::GetDomainType() const
{
  return domainconstants::kMessageInstructionType;
}

void MessageItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kTextAttribute))
  {
    SetText(instruction->GetAttributeString(domainconstants::kTextAttribute));
  }
}

void MessageItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kTextAttribute, GetText(), *instruction);
}

std::string MessageItem::GetText() const
{
  return Property<std::string>(kText);
}

void MessageItem::SetText(const std::string &value)
{
  SetProperty(kText, value);
}

// ----------------------------------------------------------------------------
// OutputItem
// ----------------------------------------------------------------------------

OutputItem::OutputItem() : InstructionItem(Type)
{
  RegisterCommonProperties();
  AddProperty(itemconstants::kDescription, std::string())->SetDisplayName("Description");
  AddProperty(kSource, std::string())->SetDisplayName("Source");
}

std::unique_ptr<mvvm::SessionItem> OutputItem::Clone(bool make_unique_id) const
{
  return std::make_unique<OutputItem>(*this, make_unique_id);
}

std::string OutputItem::GetDomainType() const
{
  return domainconstants::kOutputInstructionType;
}

void OutputItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kDescriptionAttribute))
  {
    SetDescription(instruction->GetAttributeString(domainconstants::kDescriptionAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kOutputSourceAttribute))
  {
    SetSourceVariableName(instruction->GetAttributeString(domainconstants::kOutputSourceAttribute));
  }
}

void OutputItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kOutputSourceAttribute, GetSourceVariableName(),
                       *instruction);
  AddNonEmptyAttribute(domainconstants::kDescriptionAttribute, GetDescription(), *instruction);
}

std::string OutputItem::GetSourceVariableName() const
{
  return Property<std::string>(kSource);
}

void OutputItem::SetSourceVariableName(const std::string &value)
{
  SetProperty(kSource, value);
}

std::string OutputItem::GetDescription() const
{
  return Property<std::string>(itemconstants::kDescription);
}

void OutputItem::SetDescription(const std::string &value)
{
  SetProperty(itemconstants::kDescription, value);
}

// ----------------------------------------------------------------------------
// ParallelSequenceItem
// ----------------------------------------------------------------------------

ParallelSequenceItem::ParallelSequenceItem() : InstructionItem(Type)
{
  RegisterCommonProperties();
  AddProperty(kSuccessThreshold, -1)
      ->SetDisplayName("Success threshold")
      ->SetToolTip("Negative means that all children should succeed");
  AddProperty(kFailureThreshold, 1)->SetDisplayName("Failure threshold");
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
              /*as_default*/ true);
}

std::unique_ptr<mvvm::SessionItem> ParallelSequenceItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ParallelSequenceItem>(*this, make_unique_id);
}

std::string ParallelSequenceItem::GetDomainType() const
{
  return domainconstants::kParallelInstructionType;
}

void ParallelSequenceItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kSuccessThresholdAttribute))
  {
    SetSuccessThreshold(
        std::stoi(instruction->GetAttributeString(domainconstants::kSuccessThresholdAttribute)));
  }

  if (instruction->HasAttribute(domainconstants::kFailureThresholdAttribute))
  {
    SetFailureThreshold(
        std::stoi(instruction->GetAttributeString(domainconstants::kFailureThresholdAttribute)));
  }
}

void ParallelSequenceItem::SetupDomainImpl(instruction_t *instruction) const
{
  if (GetSuccessThreshold() >= 0)
  {
    instruction->AddAttribute(domainconstants::kSuccessThresholdAttribute,
                              std::to_string(GetSuccessThreshold()));
  }
  instruction->AddAttribute(domainconstants::kFailureThresholdAttribute,
                            std::to_string(GetFailureThreshold()));
}

int ParallelSequenceItem::GetSuccessThreshold() const
{
  return Property<int>(kSuccessThreshold);
}

void ParallelSequenceItem::SetSuccessThreshold(int value)
{
  SetProperty(kSuccessThreshold, value);
}

int ParallelSequenceItem::GetFailureThreshold() const
{
  return Property<int>(kFailureThreshold);
}

void ParallelSequenceItem::SetFailureThreshold(int value)
{
  SetProperty(kFailureThreshold, value);
}

// ----------------------------------------------------------------------------
// RepeatItem
// ----------------------------------------------------------------------------

RepeatItem::RepeatItem() : InstructionItem(Type)
{
  RegisterCommonProperties();
  AddProperty(kMaxCount, -1)->SetDisplayName("maxCount");
  RegisterTag(mvvm::TagInfo(itemconstants::kChildInstructions, 0, 1, {}), /*default*/ true);
}

std::unique_ptr<mvvm::SessionItem> RepeatItem::Clone(bool make_unique_id) const
{
  return std::make_unique<RepeatItem>(*this, make_unique_id);
}

std::string RepeatItem::GetDomainType() const
{
  return domainconstants::kRepeatInstructionType;
}

void RepeatItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kMaxCountAttribute))
  {
    SetRepeatCount(std::stoi(instruction->GetAttributeString(domainconstants::kMaxCountAttribute)));
  }
  else
  {
    SetRepeatCount(-1);
  }
}

void RepeatItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(domainconstants::kMaxCountAttribute, std::to_string(GetRepeatCount()));
}

int RepeatItem::GetRepeatCount() const
{
  return Property<int>(kMaxCount);
}

void RepeatItem::SetRepeatCount(int value)
{
  SetProperty(kMaxCount, value);
}

// ----------------------------------------------------------------------------
// SequenceItem
// ----------------------------------------------------------------------------
std::unique_ptr<mvvm::SessionItem> SequenceItem::Clone(bool make_unique_id) const
{
  return std::make_unique<SequenceItem>(*this, make_unique_id);
}

SequenceItem::SequenceItem() : InstructionItem(Type)
{
  RegisterCommonProperties();
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
              /*as_default*/ true);
}

std::string SequenceItem::GetDomainType() const
{
  return domainconstants::kSequenceInstructionType;
}

void SequenceItem::InitFromDomainImpl(const instruction_t *instruction) {}

void SequenceItem::SetupDomainImpl(instruction_t *instruction) const
{
  (void)instruction;
}

// ----------------------------------------------------------------------------
// UserChoiceItem
// ----------------------------------------------------------------------------

UserChoiceItem::UserChoiceItem() : InstructionItem(Type)
{
  RegisterCommonProperties();
  AddProperty(itemconstants::kDescription, std::string())->SetDisplayName("Description");
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
              /*as_default*/ true);
}

std::unique_ptr<mvvm::SessionItem> UserChoiceItem::Clone(bool make_unique_id) const
{
  return std::make_unique<UserChoiceItem>(*this, make_unique_id);
}

std::string UserChoiceItem::GetDomainType() const
{
  return domainconstants::kUserChoiceInstructionType;
}

std::string UserChoiceItem::GetDescription() const
{
  return Property<std::string>(itemconstants::kDescription);
}

void UserChoiceItem::SetDescription(const std::string &value)
{
  SetProperty(itemconstants::kDescription, value);
}

void UserChoiceItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kDescriptionAttribute))
  {
    SetDescription(instruction->GetAttributeString(domainconstants::kDescriptionAttribute));
  }
}

void UserChoiceItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kDescriptionAttribute, GetDescription(), *instruction);
}

// ----------------------------------------------------------------------------
// VariableResetItem
// ----------------------------------------------------------------------------

VariableResetItem::VariableResetItem() : InstructionItem(Type)
{
  RegisterCommonProperties();
  AddProperty(kVariableName, std::string())->SetDisplayName("Variable name");
}

std::unique_ptr<mvvm::SessionItem> VariableResetItem::Clone(bool make_unique_id) const
{
  return std::make_unique<VariableResetItem>(*this, make_unique_id);
}

std::string VariableResetItem::GetDomainType() const
{
  return domainconstants::kVariableResetInstructionType;
}

void VariableResetItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kVarNameAttribute))
  {
    SetVariableName(instruction->GetAttributeString(domainconstants::kVarNameAttribute));
  }
}

void VariableResetItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kVarNameAttribute, GetVariableName(), *instruction);
}

std::string VariableResetItem::GetVariableName() const
{
  return Property<std::string>(kVariableName);
}

void VariableResetItem::SetVariableName(const std::string &value)
{
  SetProperty(kVariableName, value);
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
  SetProperty(kTimeout, value);
}

double WaitItem::GetTimeout() const
{
  return Property<double>(kTimeout);
}

}  // namespace sequencergui
