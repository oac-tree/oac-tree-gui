/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <oac-tree-gui/domain/domain_constants.h>
#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/transform/attribute_item_transform_helper.h>

namespace oac_tree_gui
{

// ----------------------------------------------------------------------------
// IncludeItem
// ----------------------------------------------------------------------------

IncludeItem::IncludeItem() : UniversalInstructionItem(GetStaticType()) {}

std::string IncludeItem::GetStaticType()
{
  return domainconstants::kIncludeInstructionType;
}

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

ParallelSequenceItem::ParallelSequenceItem() : UniversalInstructionItem(GetStaticType()) {}

std::string ParallelSequenceItem::GetStaticType()
{
  return domainconstants::kParallelInstructionType;
}

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

RepeatItem::RepeatItem() : UniversalInstructionItem(GetStaticType())
{
  SetRepeatCount(-1);
}

std::string RepeatItem::GetStaticType()
{
  return domainconstants::kRepeatInstructionType;
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
SequenceItem::SequenceItem() : UniversalInstructionItem(GetStaticType()) {}

std::string SequenceItem::GetStaticType()
{
  return domainconstants::kSequenceInstructionType;
}

std::unique_ptr<mvvm::SessionItem> SequenceItem::Clone() const
{
  return std::make_unique<SequenceItem>(*this);
}

// ----------------------------------------------------------------------------
// WaitItem
// ----------------------------------------------------------------------------

WaitItem::WaitItem() : UniversalInstructionItem(GetStaticType())
{
  SetTimeout(0.0);
}

std::string WaitItem::GetStaticType()
{
  return domainconstants::kWaitInstructionType;
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

}  // namespace oac_tree_gui
