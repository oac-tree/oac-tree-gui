/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
  return Property<std::string>(domainconstants::kFileNameAttribute);
}

void IncludeItem::SetFileName(const std::string &value)
{
  SetProperty(domainconstants::kFileNameAttribute, value);
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
// ParallelSequenceItem
// ----------------------------------------------------------------------------

ParallelSequenceItem::ParallelSequenceItem() : UniversalInstructionItem(Type)
{
}

std::unique_ptr<mvvm::SessionItem> ParallelSequenceItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ParallelSequenceItem>(*this, make_unique_id);
}

mvvm::uint32 ParallelSequenceItem::GetSuccessThreshold() const
{
  return Property<mvvm::uint32>(domainconstants::kSuccessThresholdAttribute);
}

void ParallelSequenceItem::SetSuccessThreshold(mvvm::uint32 value)
{
  SetAttribute(domainconstants::kSuccessThresholdAttribute, value);
}

mvvm::uint32 ParallelSequenceItem::GetFailureThreshold() const
{
  return Property<mvvm::uint32>(domainconstants::kFailureThresholdAttribute);
}

void ParallelSequenceItem::SetFailureThreshold(mvvm::uint32 value)
{
  SetAttribute(domainconstants::kFailureThresholdAttribute, value);
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
