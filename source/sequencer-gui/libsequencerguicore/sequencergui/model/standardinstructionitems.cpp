/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/standardinstructionitems.h"

#include "Instruction.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/item_constants.h"

#include "mvvm/utils/stringutils.h"

namespace sequencergui
{
// ----------------------------------------------------------------------------
// ConditionItem
// ----------------------------------------------------------------------------

static inline const std::string kVariableName = "kVariableName";

ConditionItem::ConditionItem() : InstructionItem(Type)
{
  AddProperty(kVariableName, std::string())->SetDisplayName("Variable name");
}

std::string ConditionItem::GetDomainType() const
{
  return DomainConstants::kConditionInstructionType;
}

void ConditionItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kConditionVarNameAttribute))
  {
    SetVariableName(instruction->GetAttribute(DomainConstants::kConditionVarNameAttribute));
  }
}

void ConditionItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kConditionVarNameAttribute, GetVariableName());
}

std::string ConditionItem::GetVariableName() const
{
  return Property<std::string>(kVariableName);
}

void ConditionItem::SetVariableName(const std::string &value)
{
  SetProperty(kVariableName, value);
}

// ----------------------------------------------------------------------------
// CopyItem
// ----------------------------------------------------------------------------
static inline const std::string kInput = "kInput";
static inline const std::string kOutput = "kOutput";

CopyItem::CopyItem() : InstructionItem(Type)
{
  AddProperty(kInput, std::string())->SetDisplayName("Input");
  AddProperty(kOutput, std::string())->SetDisplayName("Output");
}

std::string CopyItem::GetDomainType() const
{
  return DomainConstants::kCopyInstructionType;
}

void CopyItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (!instruction->HasAttribute(DomainConstants::kInputAttribute))
  {
    throw std::runtime_error("Error in CopyItem: no `input` attribute provided");
  }
  SetInput(instruction->GetAttribute(DomainConstants::kInputAttribute));

  if (!instruction->HasAttribute(DomainConstants::kOutputAttribute))
  {
    throw std::runtime_error("Error in CopyItem: no `output` attribute provided");
  }
  SetOutput(instruction->GetAttribute(DomainConstants::kOutputAttribute));
}

void CopyItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kInputAttribute, GetInput());
  instruction->AddAttribute(DomainConstants::kOutputAttribute, GetOutput());
}

std::string CopyItem::GetInput() const
{
  return Property<std::string>(kInput);
}

void CopyItem::SetInput(const std::string &value)
{
  SetProperty(kInput, value);
}

std::string CopyItem::GetOutput() const
{
  return Property<std::string>(kOutput);
}

void CopyItem::SetOutput(const std::string &value)
{
  SetProperty(kOutput, value);
}

// ----------------------------------------------------------------------------
// EqualsItem
// ----------------------------------------------------------------------------

static inline const std::string kLeftHandSide = "kLeftHandSide";
static inline const std::string kRightHandSide = "kRightHandSide";

EqualsItem::EqualsItem() : InstructionItem(Type)
{
  AddProperty(kLeftHandSide, std::string())->SetDisplayName("lhs");
  AddProperty(kRightHandSide, std::string())->SetDisplayName("rhs");
}

std::string EqualsItem::GetDomainType() const
{
  return DomainConstants::kEqualsInstructionType;
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
  if (instruction->HasAttribute(DomainConstants::kLeftHandAttribute))
  {
    SetLeftHandSide(instruction->GetAttribute(DomainConstants::kLeftHandAttribute));
  }
  if (instruction->HasAttribute(DomainConstants::kRightHandAttribute))
  {
    SetRightHandSide(instruction->GetAttribute(DomainConstants::kRightHandAttribute));
  }
}

void EqualsItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kLeftHandAttribute, GetLeftHandSide());
  instruction->AddAttribute(DomainConstants::kRightHandAttribute, GetRightHandSide());
}

// ----------------------------------------------------------------------------
// FallbackItem
// ----------------------------------------------------------------------------
FallbackItem::FallbackItem() : InstructionItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(ItemConstants::kChildInstructions),
              /*as_default*/ true);
}

std::string FallbackItem::GetDomainType() const
{
  return DomainConstants::kFallbackInstructionType;
}

void FallbackItem::InitFromDomainImpl(const instruction_t *instruction) {}

void FallbackItem::SetupDomainImpl(instruction_t *instruction) const
{
  (void)instruction;
}

// ----------------------------------------------------------------------------
// ForceSuccess
// ----------------------------------------------------------------------------

ForceSuccessItem::ForceSuccessItem() : InstructionItem(Type)
{
  RegisterTag(mvvm::TagInfo(ItemConstants::kChildInstructions, 0, 1, {}), /*as_default*/ true);
}

std::string ForceSuccessItem::GetDomainType() const
{
  return DomainConstants::kForceSuccessInstructionType;
}

void ForceSuccessItem::InitFromDomainImpl(const instruction_t *instruction)
{
  (void)instruction;
}

void ForceSuccessItem::SetupDomainImpl(instruction_t *instruction) const
{
  (void)instruction;
}

// ----------------------------------------------------------------------------
// IncludeItem
// ----------------------------------------------------------------------------

static inline const std::string kFile = "kFile";
static inline const std::string kPath = "kPath";

IncludeItem::IncludeItem() : InstructionItem(Type)
{
  AddProperty(kFile, std::string())->SetDisplayName("File name");
  AddProperty(kPath, std::string())->SetDisplayName("Path");
  RegisterTag(mvvm::TagInfo(ItemConstants::kChildInstructions, 0, 1, {}), /*as_default*/ true);
}

std::string IncludeItem::GetDomainType() const
{
  return DomainConstants::kIncludeInstructionType;
}

void IncludeItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kFileAttribute))
  {
    SetFileName(instruction->GetAttribute(DomainConstants::kFileAttribute));
  }

  if (instruction->HasAttribute(DomainConstants::kPathAttribute))
  {
    SetPath(instruction->GetAttribute(DomainConstants::kPathAttribute));
  }
}

void IncludeItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kFileAttribute, GetFileName());
  instruction->AddAttribute(DomainConstants::kPathAttribute, GetPath());
}

std::string IncludeItem::GetFileName() const
{
  return Property<std::string>(kFile);
}

void IncludeItem::SetFileName(const std::string &value)
{
  SetProperty(kFile, value);
}

std::string IncludeItem::GetPath() const
{
  return Property<std::string>(kPath);
}

void IncludeItem::SetPath(const std::string &value)
{
  SetProperty(kPath, value);
}

// ----------------------------------------------------------------------------
// InputItem
// ----------------------------------------------------------------------------
static inline const std::string kTarget = "kTarget";

InputItem::InputItem() : InstructionItem(Type)
{
  AddProperty(ItemConstants::kDescription, std::string())->SetDisplayName("Description");
  AddProperty(kTarget, std::string())->SetDisplayName("Target");
}

std::string InputItem::GetDomainType() const
{
  return DomainConstants::kInputInstructionType;
}

void InputItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kDescriptionAttribute))
  {
    SetDescription(instruction->GetAttribute(DomainConstants::kDescriptionAttribute));
  }

  if (instruction->HasAttribute(DomainConstants::kInputTargetAttribute))
  {
    SetTargetVariableName(instruction->GetAttribute(DomainConstants::kInputTargetAttribute));
  }
}

void InputItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kInputTargetAttribute, GetTargetVariableName());
  instruction->AddAttribute(DomainConstants::kDescriptionAttribute, GetDescription());
}

std::string InputItem::GetTargetVariableName() const
{
  return Property<std::string>(kTarget);
}

void InputItem::SetTargetVariableName(const std::string &value)
{
  SetProperty(kTarget, value);
}

std::string InputItem::GetDescription() const
{
  return Property<std::string>(ItemConstants::kDescription);
}

void InputItem::SetDescription(const std::string &value)
{
  SetProperty(ItemConstants::kDescription, value);
}

// ----------------------------------------------------------------------------
// InverterItem
// ----------------------------------------------------------------------------

InverterItem::InverterItem() : InstructionItem(Type)
{
  RegisterTag(mvvm::TagInfo(ItemConstants::kChildInstructions, 0, 1, {}), /*as_default*/ true);
}

std::string InverterItem::GetDomainType() const
{
  return DomainConstants::kInverterInstructionType;
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
static inline const std::string kForceSuccess = "kForceSuccess";
static inline const std::string kVarNames = "kVarNames";

ListenItem::ListenItem() : InstructionItem(Type)
{
  AddProperty(kForceSuccess, false)->SetDisplayName("Force success");
  AddProperty(kVarNames, std::string())->SetDisplayName("Var names");
  RegisterTag(mvvm::TagInfo(ItemConstants::kChildInstructions, 0, 1, {}), /*as_default*/ true);
}

std::string ListenItem::GetDomainType() const
{
  return DomainConstants::kListenInstructionType;
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
  if (instruction->HasAttribute(DomainConstants::kListenForceSuccessAttribute))
  {
    SetForceSuccess(::mvvm::utils::StringToBool(
        instruction->GetAttribute(DomainConstants::kListenForceSuccessAttribute)));
  }

  if (instruction->HasAttribute(DomainConstants::kListenVarNamesAttribute))
  {
    SetVarNames(instruction->GetAttribute(DomainConstants::kListenVarNamesAttribute));
  }
}

void ListenItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kListenForceSuccessAttribute,
                            mvvm::utils::FromBool(IsForceSuccess()));
  instruction->AddAttribute(DomainConstants::kListenVarNamesAttribute, GetVarNames());
}

// ----------------------------------------------------------------------------
// MessageItem
// ----------------------------------------------------------------------------

static inline const std::string kText = "kText";

MessageItem::MessageItem() : InstructionItem(Type)
{
  AddProperty(kText, std::string())->SetDisplayName("text");
}

std::string MessageItem::GetDomainType() const
{
  return DomainConstants::kMessageInstructionType;
}

void MessageItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kTextAttribute))
  {
    SetText(instruction->GetAttribute(DomainConstants::kTextAttribute));
  }
}

void MessageItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kTextAttribute, GetText());
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

static inline const std::string kSource = "kSource";

OutputItem::OutputItem() : InstructionItem(Type)
{
  AddProperty(ItemConstants::kDescription, std::string())->SetDisplayName("Description");
  AddProperty(kSource, std::string())->SetDisplayName("Source");
}

std::string OutputItem::GetDomainType() const
{
  return DomainConstants::kOutputInstructionType;
}

void OutputItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kDescriptionAttribute))
  {
    SetDescription(instruction->GetAttribute(DomainConstants::kDescriptionAttribute));
  }

  if (instruction->HasAttribute(DomainConstants::kOutputSourceAttribute))
  {
    SetSourceVariableName(instruction->GetAttribute(DomainConstants::kOutputSourceAttribute));
  }
}

void OutputItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kOutputSourceAttribute, GetSourceVariableName());
  instruction->AddAttribute(DomainConstants::kDescriptionAttribute, GetDescription());
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
  return Property<std::string>(ItemConstants::kDescription);
}

void OutputItem::SetDescription(const std::string &value)
{
  SetProperty(ItemConstants::kDescription, value);
}

// ----------------------------------------------------------------------------
// ParallelSequenceItem
// ----------------------------------------------------------------------------

static inline const std::string kSuccessThreshold = "kSuccessThreshold";
static inline const std::string kFailureThreshold = "kFailureThreshold";

ParallelSequenceItem::ParallelSequenceItem() : InstructionItem(Type)
{
  AddProperty(kSuccessThreshold, 0)->SetDisplayName("Success threshold");
  AddProperty(kFailureThreshold, 0)->SetDisplayName("Failure threshold");
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(ItemConstants::kChildInstructions),
              /*as_default*/ true);
}

std::string ParallelSequenceItem::GetDomainType() const
{
  return DomainConstants::kParallelInstructionType;
}

void ParallelSequenceItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kSuccessThresholdAttribute))
  {
    SetSuccessThreshold(
        std::stoi(instruction->GetAttribute(DomainConstants::kSuccessThresholdAttribute)));
  }

  if (instruction->HasAttribute(DomainConstants::kFailureThresholdAttribute))
  {
    SetFailureThreshold(
        std::stoi(instruction->GetAttribute(DomainConstants::kFailureThresholdAttribute)));
  }
}

void ParallelSequenceItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kSuccessThresholdAttribute,
                            std::to_string(GetSuccessThreshold()));
  instruction->AddAttribute(DomainConstants::kFailureThresholdAttribute,
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
static inline const std::string kMaxCount = "kRepeatCount";

RepeatItem::RepeatItem() : InstructionItem(Type)
{
  AddProperty(kMaxCount, -1)->SetDisplayName("maxCount");
  RegisterTag(mvvm::TagInfo(ItemConstants::kChildInstructions, 0, 1, {}), /*default*/ true);
}

std::string RepeatItem::GetDomainType() const
{
  return DomainConstants::kRepeatInstructionType;
}

void RepeatItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kMaxCountAttribute))
  {
    SetRepeatCount(std::stoi(instruction->GetAttribute(DomainConstants::kMaxCountAttribute)));
  }
  else
  {
    SetRepeatCount(-1);
  }
}

void RepeatItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kMaxCountAttribute, std::to_string(GetRepeatCount()));
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
SequenceItem::SequenceItem() : InstructionItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(ItemConstants::kChildInstructions),
              /*as_default*/ true);
}

std::string SequenceItem::GetDomainType() const
{
  return DomainConstants::kSequenceInstructionType;
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
  AddProperty(ItemConstants::kDescription, std::string())->SetDisplayName("Description");
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(ItemConstants::kChildInstructions),
              /*as_default*/ true);
}

std::string UserChoiceItem::GetDomainType() const
{
  return DomainConstants::kUserChoiceInstructionType;
}

std::string UserChoiceItem::GetDescription() const
{
  return Property<std::string>(ItemConstants::kDescription);
}

void UserChoiceItem::SetDescription(const std::string &value)
{
  SetProperty(ItemConstants::kDescription, value);
}

void UserChoiceItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kDescriptionAttribute))
  {
    SetDescription(instruction->GetAttribute(DomainConstants::kDescriptionAttribute));
  }
}

void UserChoiceItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kDescriptionAttribute, GetDescription());
}

// ----------------------------------------------------------------------------
// WaitItem
// ----------------------------------------------------------------------------
static inline const std::string kTimeout = "kTimeout";

WaitItem::WaitItem() : InstructionItem(Type)
{
  AddProperty(kTimeout, 0.0)->SetDisplayName("Timeout");
}

std::string WaitItem::GetDomainType() const
{
  return DomainConstants::kWaitInstructionType;
}

void WaitItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kWaitTimeoutAttribute))
  {
    SetTimeout(std::stod(instruction->GetAttribute(DomainConstants::kWaitTimeoutAttribute)));
  }
  else
  {
    SetTimeout(0.0);
  }
}

void WaitItem::SetupDomainImpl(instruction_t *instruction) const
{
  instruction->AddAttribute(DomainConstants::kWaitTimeoutAttribute,
                            mvvm::utils::DoubleToString(GetTimeout()));
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
