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
  if (instruction->GetType() != GetDomainType()
      && GetType() != UnknownInstructionItem::Type)
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
// FallbackItem
// ----------------------------------------------------------------------------
FallbackItem::FallbackItem() : InstructionItem(Type)
{
  RegisterTag(ModelView::TagInfo::CreateUniversalTag(kChildInstructions), /*as_default*/ true);
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
// InputItem
// ----------------------------------------------------------------------------

static inline const std::string kFile = "kFile";
static inline const std::string kPath = "kPath";

IncludeItem::IncludeItem() : InstructionItem(Type)
{
  AddProperty(kFile, std::string())->SetDisplayName("File name");
  AddProperty(kPath, std::string())->SetDisplayName("Path");
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
  AddProperty(kDescription, std::string())->SetDisplayName("Description");
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
  return Property<std::string>(kDescription);
}

void InputItem::SetDescription(const std::string &value)
{
  SetProperty(kDescription, value);
}

// ----------------------------------------------------------------------------
// InverterItem
// ----------------------------------------------------------------------------

InverterItem::InverterItem() : InstructionItem(Type)
{
  RegisterTag(ModelView::TagInfo(kChildInstructions, 0, 1, {}), /*as_default*/ true);
}

std::string InverterItem::GetDomainType() const
{
  return DomainConstants::kInverterInstructionType;
}

void InverterItem::InitFromDomainImpl(const instruction_t *instruction) {}

void InverterItem::SetupDomainImpl(instruction_t *instruction) const
{
  (void)instruction;
}

// ----------------------------------------------------------------------------
// OutputItem
// ----------------------------------------------------------------------------

static inline const std::string kSource = "kSource";

OutputItem::OutputItem() : InstructionItem(Type)
{
  AddProperty(kDescription, std::string())->SetDisplayName("Description");
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
  return Property<std::string>(kDescription);
}

void OutputItem::SetDescription(const std::string &value)
{
  SetProperty(kDescription, value);
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
  RegisterTag(ModelView::TagInfo::CreateUniversalTag(kChildInstructions), /*as_default*/ true);
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
  RegisterTag(ModelView::TagInfo(kChildInstructions, 0, 1, {}), /*default*/ true);
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
  RegisterTag(ModelView::TagInfo::CreateUniversalTag(kChildInstructions), /*as_default*/ true);
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
                            ModelView::Utils::DoubleToString(GetTimeout()));
}

void WaitItem::SetTimeout(double value)
{
  SetProperty(kTimeout, value);
}

double WaitItem::GetTimeout() const
{
  return Property<double>(kTimeout);
}

}  // namespace sequi
