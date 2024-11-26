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
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/transform/attribute_item_transform_helper.h>

namespace sequencergui
{

// ----------------------------------------------------------------------------
// IncludeItem
// ----------------------------------------------------------------------------

IncludeItem::IncludeItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> IncludeItem::Clone() const
{
  return std::make_unique<IncludeItem>(*this);
}

std::string IncludeItem::GetFileName() const
{
  return Property<std::string>(domainconstants::kFileNameAttribute);
}

void IncludeItem::SetFileName(const std::string &value)
{
  SetAttribute(*this, domainconstants::kFileNameAttribute, value);
}

std::string IncludeItem::GetPath() const
{
  return Property<std::string>(domainconstants::kPathAttribute);
}

void IncludeItem::SetPath(const std::string &value)
{
  SetAttribute(*this, domainconstants::kPathAttribute, value);
}

// ----------------------------------------------------------------------------
// ParallelSequenceItem
// ----------------------------------------------------------------------------

ParallelSequenceItem::ParallelSequenceItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> ParallelSequenceItem::Clone() const
{
  return std::make_unique<ParallelSequenceItem>(*this);
}

mvvm::uint32 ParallelSequenceItem::GetSuccessThreshold() const
{
  return Property<mvvm::uint32>(domainconstants::kSuccessThresholdAttribute);
}

void ParallelSequenceItem::SetSuccessThreshold(mvvm::uint32 value)
{
  SetAttribute(*this, domainconstants::kSuccessThresholdAttribute, value);
}

mvvm::uint32 ParallelSequenceItem::GetFailureThreshold() const
{
  return Property<mvvm::uint32>(domainconstants::kFailureThresholdAttribute);
}

void ParallelSequenceItem::SetFailureThreshold(mvvm::uint32 value)
{
  SetAttribute(*this, domainconstants::kFailureThresholdAttribute, value);
}

// ----------------------------------------------------------------------------
// RepeatItem
// ----------------------------------------------------------------------------

RepeatItem::RepeatItem() : UniversalInstructionItem(Type)
{
  SetRepeatCount(-1);
}

std::unique_ptr<mvvm::SessionItem> RepeatItem::Clone() const
{
  return std::make_unique<RepeatItem>(*this);
}

int RepeatItem::GetRepeatCount() const
{
  return Property<int>(domainconstants::kMaxCountAttribute);
}

void RepeatItem::SetRepeatCount(int value)
{
  SetAttribute(*this, domainconstants::kMaxCountAttribute, value);
}

// ----------------------------------------------------------------------------
// SequenceItem
// ----------------------------------------------------------------------------
SequenceItem::SequenceItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> SequenceItem::Clone() const
{
  return std::make_unique<SequenceItem>(*this);
}

// ----------------------------------------------------------------------------
// WaitItem
// ----------------------------------------------------------------------------

WaitItem::WaitItem() : UniversalInstructionItem(Type)
{
  SetTimeout(0.0);
}

std::unique_ptr<mvvm::SessionItem> WaitItem::Clone() const
{
  return std::make_unique<WaitItem>(*this);
}

void WaitItem::SetTimeout(double value)
{
  SetAttribute(*this, domainconstants::kTimeoutAttribute, value);
}

double WaitItem::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

}  // namespace sequencergui
