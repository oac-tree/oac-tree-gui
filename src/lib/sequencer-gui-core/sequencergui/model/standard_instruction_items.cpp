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
