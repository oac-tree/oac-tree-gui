/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/model/instructionitems.h"

#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/domainutils.h"

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

InstructionItem::InstructionItem(const std::string &item_type) : CompoundItem(item_type)
{
  AddProperty(kName, std::string())->SetDisplayName("Name");
  AddProperty(kStatus, std::string())->SetDisplayName("Status");
  AddProperty(kXpos, 0.0)->SetDisplayName("X");
  AddProperty(kYpos, 0.0)->SetDisplayName("Y");
}

void InstructionItem::InitFromDomain(const instruction_t *instruction)
{
  if (instruction->HasAttribute(DomainConstants::kNameAttribute))
  {
    SetProperty(kName, instruction->GetAttribute(DomainConstants::kNameAttribute));
  }
}

std::unique_ptr<instruction_t> InstructionItem::CreateDomainInstruction() const
{
  throw std::runtime_error("Error in InstructionItem: method is not implemented");
}

std::vector<InstructionItem *> InstructionItem::GetInstructions() const
{
  return GetTaggedItems()->HasTag(kChildInstructions)
             ? GetItems<InstructionItem>(kChildInstructions)
             : std::vector<InstructionItem *>();
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
// CopyItem
// ----------------------------------------------------------------------------
static inline const std::string kInput = "kInput";
static inline const std::string kOutput = "kOutput";

CopyItem::CopyItem() : InstructionItem(Type)
{
  AddProperty(kInput, std::string())->SetDisplayName("Input");
  AddProperty(kOutput, std::string())->SetDisplayName("Output");
}

void CopyItem::InitFromDomain(const instruction_t *instruction)
{
  InstructionItem::InitFromDomain(instruction);

  if (instruction->GetType() != DomainConstants::kCopyInstructionType)
  {
    throw std::runtime_error("Error in CopyItem: instruction doesn't match presentation item.");
  }

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

std::unique_ptr<instruction_t> CopyItem::CreateDomainInstruction() const
{
  auto result = DomainUtils::CreateDomainInstruction(DomainConstants::kCopyInstructionType);
  result->AddAttribute(DomainConstants::kInputAttribute, GetInput());
  result->AddAttribute(DomainConstants::kOutputAttribute, GetOutput());
  return result;
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

std::unique_ptr<instruction_t> FallbackItem::CreateDomainInstruction() const
{
  return DomainUtils::CreateDomainInstruction(DomainConstants::kFallbackInstructionType);
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

void InputItem::InitFromDomain(const instruction_t *instruction)
{
  InstructionItem::InitFromDomain(instruction);

  if (instruction->GetType() != DomainConstants::kInputInstructionType)
  {
    throw std::runtime_error("Error in InputItem: instruction doesn't match presentation item.");
  }

  if (instruction->HasAttribute(DomainConstants::kDescriptionAttribute))
  {
    SetDescription(instruction->GetAttribute(DomainConstants::kDescriptionAttribute));
  }

  if (instruction->HasAttribute(DomainConstants::kInputTargetAttribute))
  {
    SetTargetVariableName(instruction->GetAttribute(DomainConstants::kInputTargetAttribute));
  }
}

std::unique_ptr<instruction_t> InputItem::CreateDomainInstruction() const
{
  auto result = DomainUtils::CreateDomainInstruction(DomainConstants::kInputInstructionType);

  result->AddAttribute(DomainConstants::kInputTargetAttribute, GetTargetVariableName());
  result->AddAttribute(DomainConstants::kDescriptionAttribute, GetDescription());

  return result;
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

std::unique_ptr<instruction_t> InverterItem::CreateDomainInstruction() const
{
  return DomainUtils::CreateDomainInstruction(DomainConstants::kInverterInstructionType);
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

void OutputItem::InitFromDomain(const instruction_t *instruction)
{
  InstructionItem::InitFromDomain(instruction);

  if (instruction->GetType() != DomainConstants::kOutputInstructionType)
  {
    throw std::runtime_error("Error in InpuItem: instruction doesn't match presentation item.");
  }

  if (instruction->HasAttribute(DomainConstants::kDescriptionAttribute))
  {
    SetDescription(instruction->GetAttribute(DomainConstants::kDescriptionAttribute));
  }

  if (instruction->HasAttribute(DomainConstants::kOutputSourceAttribute))
  {
    SetSourceVariableName(instruction->GetAttribute(DomainConstants::kOutputSourceAttribute));
  }
}

std::unique_ptr<instruction_t> OutputItem::CreateDomainInstruction() const
{
  auto result = DomainUtils::CreateDomainInstruction(DomainConstants::kOutputInstructionType);

  result->AddAttribute(DomainConstants::kOutputSourceAttribute, GetSourceVariableName());
  result->AddAttribute(DomainConstants::kDescriptionAttribute, GetDescription());

  return result;
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
// RepeatItem
// ----------------------------------------------------------------------------
static inline const std::string kMaxCount = "kRepeatCount";

RepeatItem::RepeatItem() : InstructionItem(Type)
{
  AddProperty(kMaxCount, -1)->SetDisplayName("maxCount");
  RegisterTag(ModelView::TagInfo(kChildInstructions, 0, 1, {}), /*default*/ true);
}

void RepeatItem::InitFromDomain(const instruction_t *instruction)
{
  InstructionItem::InitFromDomain(instruction);

  if (instruction->GetType() != DomainConstants::kRepeatInstructionType)
  {
    throw std::runtime_error("Error in RepeatItem: instruction doesn't match presentation item.");
  }

  if (instruction->HasAttribute(DomainConstants::kMaxCountAttribute))
  {
    SetRepeatCount(std::stoi(instruction->GetAttribute(DomainConstants::kMaxCountAttribute)));
  }
  else
  {
    SetRepeatCount(-1);
  }
}

std::unique_ptr<instruction_t> RepeatItem::CreateDomainInstruction() const
{
  auto result = DomainUtils::CreateDomainInstruction(DomainConstants::kRepeatInstructionType);
  result->AddAttribute(DomainConstants::kMaxCountAttribute, std::to_string(GetRepeatCount()));

  return result;
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

void SequenceItem::InitFromDomain(const instruction_t *instruction)
{
  InstructionItem::InitFromDomain(instruction);
  if (instruction->GetType() != DomainConstants::kSequenceInstructionType)
  {
    throw std::runtime_error("Error in SequenceItem: instruction doesn't match presentation item.");
  }
}

std::unique_ptr<instruction_t> SequenceItem::CreateDomainInstruction() const
{
  return DomainUtils::CreateDomainInstruction(DomainConstants::kSequenceInstructionType);
}

// ----------------------------------------------------------------------------
// WaitItem
// ----------------------------------------------------------------------------
static inline const std::string kTimeout = "kTimeout";

WaitItem::WaitItem() : InstructionItem(Type)
{
  AddProperty(kTimeout, 0.0)->SetDisplayName("Timeout");
}

void WaitItem::InitFromDomain(const instruction_t *instruction)
{
  InstructionItem::InitFromDomain(instruction);

  if (instruction->GetType() != DomainConstants::kWaitInstructionType)
  {
    throw std::runtime_error("Error in WaitItem: instruction doesn't match presentation item.");
  }

  if (instruction->HasAttribute(DomainConstants::kWaitTimeoutAttribute))
  {
    SetTimeout(std::stod(instruction->GetAttribute(DomainConstants::kWaitTimeoutAttribute)));
  }
  else
  {
    SetTimeout(0.0);
  }
}

std::unique_ptr<instruction_t> WaitItem::CreateDomainInstruction() const
{
  auto result = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);

  result->AddAttribute(DomainConstants::kWaitTimeoutAttribute,
                       ModelView::Utils::DoubleToString(GetTimeout()));
  return result;
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
