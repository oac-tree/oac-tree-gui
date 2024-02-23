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

#include "epics_instruction_items.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/attribute_item_transform_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>

namespace
{
void InitDefaultAnyValue(mvvm::CompoundItem &item)
{
  item.RegisterTag(sup::gui::CreateAnyValueTag(sequencergui::itemconstants::kAnyValueTag), true);
  auto anyvalue_item = item.InsertItem<sup::gui::AnyValueEmptyItem>(mvvm::TagIndex::Append());
  anyvalue_item->SetDisplayName(sequencergui::itemconstants::kAnyValueDefaultDisplayName);
  sequencergui::SetAttributePresentFlag(false, *anyvalue_item);
}

}  // namespace

namespace sequencergui
{

// ----------------------------------------------------------------------------
// EpicsReadInstructionItem
// ----------------------------------------------------------------------------

EpicsReadInstructionItem::EpicsReadInstructionItem(const std::string &instruction_type)
    : UniversalInstructionItem(instruction_type)
{
  SetTimeout(1.0);
}

std::unique_ptr<mvvm::SessionItem> EpicsReadInstructionItem::Clone(bool make_unique_id) const
{
  (void)make_unique_id;
  throw NotImplementedException("EpicsReadInstructionItem::clone() should not be used");
}

std::string EpicsReadInstructionItem::GetChannel() const
{
  return Property<std::string>(domainconstants::kChannelAttribute);
}

void EpicsReadInstructionItem::SetChannel(const std::string &value)
{
  SetAttribute(domainconstants::kChannelAttribute, value);
}

std::string EpicsReadInstructionItem::GetOutput() const
{
  return Property<std::string>(domainconstants::kOutputVariableNameAttribute);
}

void EpicsReadInstructionItem::SetOutput(const std::string &value)
{
  SetAttribute(domainconstants::kOutputVariableNameAttribute, value);
}

double EpicsReadInstructionItem::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

void EpicsReadInstructionItem::SetTimeout(double value)
{
  SetAttribute(domainconstants::kTimeoutAttribute, value);
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

std::unique_ptr<mvvm::SessionItem> EpicsWriteInstructionItem::Clone(bool make_unique_id) const
{
  (void)make_unique_id;
  throw NotImplementedException("EpicsWriteInstructionItem::clone() should not be used");
}

std::string EpicsWriteInstructionItem::GetVariableName() const
{
  return Property<std::string>(domainconstants::kGenericVariableNameAttribute);
}

void EpicsWriteInstructionItem::SetVariableName(const std::string &value)
{
  SetAttribute(domainconstants::kGenericVariableNameAttribute, value);
}

std::string EpicsWriteInstructionItem::GetChannel() const
{
  return Property<std::string>(domainconstants::kChannelAttribute);
}

void EpicsWriteInstructionItem::SetChannel(const std::string &value)
{
  SetAttribute(domainconstants::kChannelAttribute, value);
}

double EpicsWriteInstructionItem::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

void EpicsWriteInstructionItem::SetTimeout(double value)
{
  SetAttribute(domainconstants::kTimeoutAttribute, value);
}

// ----------------------------------------------------------------------------
// ChannelAccessReadInstructionItem
// ----------------------------------------------------------------------------

ChannelAccessReadInstructionItem::ChannelAccessReadInstructionItem()
    : EpicsReadInstructionItem(Type)
{
}

std::unique_ptr<mvvm::SessionItem> ChannelAccessReadInstructionItem::Clone(
    bool make_unique_id) const
{
  return std::make_unique<ChannelAccessReadInstructionItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// ChannelAccessWriteInstructionItem
// ----------------------------------------------------------------------------

ChannelAccessWriteInstructionItem::ChannelAccessWriteInstructionItem()
    : EpicsWriteInstructionItem(Type)
{
}

std::unique_ptr<mvvm::SessionItem> ChannelAccessWriteInstructionItem::Clone(
    bool make_unique_id) const
{
  return std::make_unique<ChannelAccessWriteInstructionItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// PVAccessReadInstructionItem
// ----------------------------------------------------------------------------

PvAccessReadInstructionItem::PvAccessReadInstructionItem() : EpicsReadInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> PvAccessReadInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<PvAccessReadInstructionItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// PVAccessWriteInstructionItem
// ----------------------------------------------------------------------------

PvAccessWriteInstructionItem::PvAccessWriteInstructionItem() : EpicsWriteInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> PvAccessWriteInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<PvAccessWriteInstructionItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// RPCClientInstruction
// ----------------------------------------------------------------------------

RPCClientInstruction::RPCClientInstruction() : UniversalInstructionItem(Type)
{
  SetTimeout(1.0);
  InitDefaultAnyValue(*this);
}

std::unique_ptr<mvvm::SessionItem> RPCClientInstruction::Clone(bool make_unique_id) const
{
  return std::make_unique<RPCClientInstruction>(*this, make_unique_id);
}
std::string RPCClientInstruction::GetService() const
{
  return Property<std::string>(domainconstants::kServiceAttribute);
}

void RPCClientInstruction::SetService(const std::string &value)
{
  SetAttribute(domainconstants::kServiceAttribute, value);
}

std::string RPCClientInstruction::GetRequestVar() const
{
  return Property<std::string>(domainconstants::kRequestAttribute);
}

void RPCClientInstruction::SetRequestVar(const std::string &value)
{
  SetAttribute(domainconstants::kRequestAttribute, value);
}

double RPCClientInstruction::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

void RPCClientInstruction::SetTimeout(double value)
{
  SetAttribute(domainconstants::kTimeoutAttribute, value);
}

std::string RPCClientInstruction::GetOutput() const
{
  return Property<std::string>(domainconstants::kOutputVariableNameAttribute);
}

void RPCClientInstruction::SetOutput(const std::string &value)
{
  SetAttribute(domainconstants::kOutputVariableNameAttribute, value);
}

// ----------------------------------------------------------------------------
// SystemCallInstructionItem
// ----------------------------------------------------------------------------

SystemCallInstructionItem::SystemCallInstructionItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> SystemCallInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<SystemCallInstructionItem>(*this, make_unique_id);
}

std::string SystemCallInstructionItem::GetCommand() const
{
  return Property<std::string>(domainconstants::kCommandAttribute);
}

void SystemCallInstructionItem::SetCommand(const std::string &value)
{
  SetAttribute(domainconstants::kCommandAttribute, value);
}

// ----------------------------------------------------------------------------
// LogInstructionItem
// ----------------------------------------------------------------------------

LogInstructionItem::LogInstructionItem() : UniversalInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> LogInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LogInstructionItem>(*this, make_unique_id);
}

std::string LogInstructionItem::GetMessage() const
{
  return Property<std::string>(domainconstants::kMessageAttribute);
}

void LogInstructionItem::SetMessage(const std::string &value)
{
  SetAttribute(domainconstants::kMessageAttribute, value);
}

std::string LogInstructionItem::GetInput() const
{
  return Property<std::string>(domainconstants::kInputVariableNameAttribute);
}

void LogInstructionItem::SetInput(const std::string &value)
{
  SetAttribute(domainconstants::kInputVariableNameAttribute, value);
}

std::string LogInstructionItem::GetSeverity() const
{
  return Property<std::string>(domainconstants::kSeverityAttribute);
}

void LogInstructionItem::SetSeverity(const std::string &value)
{
  SetAttribute(domainconstants::kSeverityAttribute, value);
}

}  // namespace sequencergui
