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

#include "epics_instruction_items.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>

namespace
{
void InitDefaultAnyValue(mvvm::CompoundItem &item)
{
  item.RegisterTag(sup::gui::CreateAnyValueTag(oac_tree_gui::itemconstants::kAnyValueTag), true);
  auto anyvalue_item = item.InsertItem<sup::gui::AnyValueEmptyItem>(mvvm::TagIndex::Append());
  anyvalue_item->SetDisplayName(oac_tree_gui::itemconstants::kAnyValueDefaultDisplayName);
  oac_tree_gui::SetAttributeExposedFlag(false, *anyvalue_item);
}

}  // namespace

namespace oac_tree_gui
{

// ----------------------------------------------------------------------------
// EpicsReadInstructionItem
// ----------------------------------------------------------------------------

EpicsReadInstructionItem::EpicsReadInstructionItem(const std::string &instruction_type)
    : UniversalInstructionItem(instruction_type)
{
  SetTimeout(1.0);
}

std::unique_ptr<mvvm::SessionItem> EpicsReadInstructionItem::Clone() const
{
  throw NotImplementedException("EpicsReadInstructionItem::clone() should not be used");
}

std::string EpicsReadInstructionItem::GetChannel() const
{
  return Property<std::string>(domainconstants::kChannelAttribute);
}

void EpicsReadInstructionItem::SetChannel(const std::string &value)
{
  SetAttribute(*this, domainconstants::kChannelAttribute, value);
}

std::string EpicsReadInstructionItem::GetOutput() const
{
  return Property<std::string>(domainconstants::kOutputVariableNameAttribute);
}

void EpicsReadInstructionItem::SetOutput(const std::string &value)
{
  SetAttribute(*this, domainconstants::kOutputVariableNameAttribute, value);
}

double EpicsReadInstructionItem::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

void EpicsReadInstructionItem::SetTimeout(double value)
{
  SetAttribute(*this, domainconstants::kTimeoutAttribute, value);
}

// ----------------------------------------------------------------------------
// EpicsWriteInstructionItem
// ----------------------------------------------------------------------------

EpicsWriteInstructionItem::EpicsWriteInstructionItem(const std::string &instruction_type)
    : UniversalInstructionItem(instruction_type)
{
  SetTimeout(1.0);
  InitDefaultAnyValue(*this);
}

std::unique_ptr<mvvm::SessionItem> EpicsWriteInstructionItem::Clone() const
{
  throw NotImplementedException("EpicsWriteInstructionItem::clone() should not be used");
}

std::string EpicsWriteInstructionItem::GetVariableName() const
{
  return Property<std::string>(domainconstants::kGenericVariableNameAttribute);
}

void EpicsWriteInstructionItem::SetVariableName(const std::string &value)
{
  SetAttribute(*this, domainconstants::kGenericVariableNameAttribute, value);
}

std::string EpicsWriteInstructionItem::GetChannel() const
{
  return Property<std::string>(domainconstants::kChannelAttribute);
}

void EpicsWriteInstructionItem::SetChannel(const std::string &value)
{
  SetAttribute(*this, domainconstants::kChannelAttribute, value);
}

double EpicsWriteInstructionItem::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

void EpicsWriteInstructionItem::SetTimeout(double value)
{
  SetAttribute(*this, domainconstants::kTimeoutAttribute, value);
}

// ----------------------------------------------------------------------------
// ChannelAccessReadInstructionItem
// ----------------------------------------------------------------------------

ChannelAccessReadInstructionItem::ChannelAccessReadInstructionItem()
    : EpicsReadInstructionItem(mvvm::GetTypeName<ChannelAccessReadInstructionItem>())
{
}

std::unique_ptr<mvvm::SessionItem> ChannelAccessReadInstructionItem::Clone() const
{
  return std::make_unique<ChannelAccessReadInstructionItem>(*this);
}

// ----------------------------------------------------------------------------
// ChannelAccessWriteInstructionItem
// ----------------------------------------------------------------------------

ChannelAccessWriteInstructionItem::ChannelAccessWriteInstructionItem()
    : EpicsWriteInstructionItem(mvvm::GetTypeName<ChannelAccessWriteInstructionItem>())
{
}

std::unique_ptr<mvvm::SessionItem> ChannelAccessWriteInstructionItem::Clone() const
{
  return std::make_unique<ChannelAccessWriteInstructionItem>(*this);
}

// ----------------------------------------------------------------------------
// PVAccessReadInstructionItem
// ----------------------------------------------------------------------------

PvAccessReadInstructionItem::PvAccessReadInstructionItem()
    : EpicsReadInstructionItem(mvvm::GetTypeName<PvAccessReadInstructionItem>())
{
}

std::unique_ptr<mvvm::SessionItem> PvAccessReadInstructionItem::Clone() const
{
  return std::make_unique<PvAccessReadInstructionItem>(*this);
}

// ----------------------------------------------------------------------------
// PVAccessWriteInstructionItem
// ----------------------------------------------------------------------------

PvAccessWriteInstructionItem::PvAccessWriteInstructionItem()
    : EpicsWriteInstructionItem(mvvm::GetTypeName<PvAccessWriteInstructionItem>())
{
}

std::unique_ptr<mvvm::SessionItem> PvAccessWriteInstructionItem::Clone() const
{
  return std::make_unique<PvAccessWriteInstructionItem>(*this);
}

// ----------------------------------------------------------------------------
// RPCClientInstruction
// ----------------------------------------------------------------------------

RPCClientInstruction::RPCClientInstruction()
    : UniversalInstructionItem(mvvm::GetTypeName<RPCClientInstruction>())
{
  SetTimeout(1.0);
  InitDefaultAnyValue(*this);
}

std::unique_ptr<mvvm::SessionItem> RPCClientInstruction::Clone() const
{
  return std::make_unique<RPCClientInstruction>(*this);
}
std::string RPCClientInstruction::GetService() const
{
  return Property<std::string>(domainconstants::kServiceAttribute);
}

void RPCClientInstruction::SetService(const std::string &value)
{
  SetAttribute(*this, domainconstants::kServiceAttribute, value);
}

std::string RPCClientInstruction::GetRequestVar() const
{
  return Property<std::string>(domainconstants::kRequestAttribute);
}

void RPCClientInstruction::SetRequestVar(const std::string &value)
{
  SetAttribute(*this, domainconstants::kRequestAttribute, value);
}

double RPCClientInstruction::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

void RPCClientInstruction::SetTimeout(double value)
{
  SetAttribute(*this, domainconstants::kTimeoutAttribute, value);
}

std::string RPCClientInstruction::GetOutput() const
{
  return Property<std::string>(domainconstants::kOutputVariableNameAttribute);
}

void RPCClientInstruction::SetOutput(const std::string &value)
{
  SetAttribute(*this, domainconstants::kOutputVariableNameAttribute, value);
}

// ----------------------------------------------------------------------------
// LogInstructionItem
// ----------------------------------------------------------------------------

LogInstructionItem::LogInstructionItem()
    : UniversalInstructionItem(mvvm::GetTypeName<LogInstructionItem>())
{
}

std::unique_ptr<mvvm::SessionItem> LogInstructionItem::Clone() const
{
  return std::make_unique<LogInstructionItem>(*this);
}

std::string LogInstructionItem::GetMessage() const
{
  return Property<std::string>(domainconstants::kMessageAttribute);
}

void LogInstructionItem::SetMessage(const std::string &value)
{
  SetAttribute(*this, domainconstants::kMessageAttribute, value);
}

std::string LogInstructionItem::GetInput() const
{
  return Property<std::string>(domainconstants::kInputVariableNameAttribute);
}

void LogInstructionItem::SetInput(const std::string &value)
{
  SetAttribute(*this, domainconstants::kInputVariableNameAttribute, value);
}

std::string LogInstructionItem::GetSeverity() const
{
  return Property<std::string>(domainconstants::kSeverityAttribute);
}

void LogInstructionItem::SetSeverity(const std::string &value)
{
  SetAttribute(*this, domainconstants::kSeverityAttribute, value);
}

}  // namespace oac_tree_gui
