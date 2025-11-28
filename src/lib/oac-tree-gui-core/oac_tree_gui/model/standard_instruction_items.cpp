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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "standard_instruction_items.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

namespace oac_tree_gui
{

// ----------------------------------------------------------------------------
// IncludeItem
// ----------------------------------------------------------------------------

IncludeItem::IncludeItem() : UniversalInstructionItem(mvvm::GetTypeName<IncludeItem>()) {}

std::unique_ptr<mvvm::SessionItem> IncludeItem::Clone() const
{
  return std::make_unique<IncludeItem>(*this);
}

std::string IncludeItem::GetFileName() const
{
  return Property<std::string>(domainconstants::kFileNameAttribute);
}

void IncludeItem::SetFileName(const std::string& value)
{
  SetAttribute(*this, domainconstants::kFileNameAttribute, value);
}

std::string IncludeItem::GetPath() const
{
  return Property<std::string>(domainconstants::kPathAttribute);
}

void IncludeItem::SetPath(const std::string& value)
{
  SetAttribute(*this, domainconstants::kPathAttribute, value);
}

// ----------------------------------------------------------------------------
// ParallelSequenceItem
// ----------------------------------------------------------------------------

ParallelSequenceItem::ParallelSequenceItem()
    : UniversalInstructionItem(mvvm::GetTypeName<ParallelSequenceItem>())
{
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

RepeatItem::RepeatItem() : UniversalInstructionItem(mvvm::GetTypeName<RepeatItem>())
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
SequenceItem::SequenceItem() : UniversalInstructionItem(mvvm::GetTypeName<SequenceItem>()) {}

std::unique_ptr<mvvm::SessionItem> SequenceItem::Clone() const
{
  return std::make_unique<SequenceItem>(*this);
}

// ----------------------------------------------------------------------------
// WaitItem
// ----------------------------------------------------------------------------

WaitItem::WaitItem() : UniversalInstructionItem(mvvm::GetTypeName<WaitItem>())
{
  SetTimeout(0.0);
  SetBlocking(false);
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

bool WaitItem::IsBlocking() const
{
  return Property<bool>(domainconstants::kBlockingAttribute);
}

void WaitItem::SetBlocking(bool value)
{
  SetAttribute(*this, domainconstants::kBlockingAttribute, value);
}

}  // namespace oac_tree_gui
