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
  SetProperty(domainconstants::kChannelAttribute, value);
}

std::string EpicsReadInstructionItem::GetOutput() const
{
  return Property<std::string>(domainconstants::kOutputVariableNameAttribute);
}

void EpicsReadInstructionItem::SetOutput(const std::string &value)
{
  SetProperty(domainconstants::kOutputVariableNameAttribute, value);
}

double EpicsReadInstructionItem::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

void EpicsReadInstructionItem::SetTimeout(double value)
{
  SetProperty(domainconstants::kTimeoutAttribute, value);
}

// ----------------------------------------------------------------------------
// EpicsWriteInstructionItem
// ----------------------------------------------------------------------------

EpicsWriteInstructionItem::EpicsWriteInstructionItem(const std::string &instruction_type)
    : UniversalInstructionItem(instruction_type)
{
  SetTimeout(1.0);
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
  SetProperty(domainconstants::kGenericVariableNameAttribute, value);
}

std::string EpicsWriteInstructionItem::GetChannel() const
{
  return Property<std::string>(domainconstants::kChannelAttribute);
}

void EpicsWriteInstructionItem::SetChannel(const std::string &value)
{
  SetProperty(domainconstants::kChannelAttribute, value);
}

double EpicsWriteInstructionItem::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

void EpicsWriteInstructionItem::SetTimeout(double value)
{
  SetProperty(domainconstants::kTimeoutAttribute, value);
}

std::string EpicsWriteInstructionItem::GetJsonType() const
{
  return Property<std::string>(domainconstants::kTypeAttribute);
}

void EpicsWriteInstructionItem::SetJsonType(const std::string &value)
{
  SetProperty(domainconstants::kTypeAttribute, value);
}

std::string EpicsWriteInstructionItem::GetJsonValue() const
{
  return Property<std::string>(domainconstants::kValueAttribute);
}

void EpicsWriteInstructionItem::SetJsonValue(const std::string &value)
{
  SetProperty(domainconstants::kValueAttribute, value);
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
  SetProperty(domainconstants::kServiceAttribute, value);
}

std::string RPCClientInstruction::GetRequestVar() const
{
  return Property<std::string>(domainconstants::kRequestAttribute);
}

void RPCClientInstruction::SetRequestVar(const std::string &value)
{
  SetProperty(domainconstants::kRequestAttribute, value);
}

double RPCClientInstruction::GetTimeout() const
{
  return Property<double>(domainconstants::kTimeoutAttribute);
}

void RPCClientInstruction::SetTimeout(double value)
{
  SetProperty(domainconstants::kTimeoutAttribute, value);
}

std::string RPCClientInstruction::GetJsonType() const
{
  return Property<std::string>(domainconstants::kTypeAttribute);
}

void RPCClientInstruction::SetJsonType(const std::string &value)
{
  SetProperty(domainconstants::kTypeAttribute, value);
}

std::string RPCClientInstruction::GetJsonValue() const
{
  return Property<std::string>(domainconstants::kValueAttribute);
}

void RPCClientInstruction::SetJsonValue(const std::string &value)
{
  SetProperty(domainconstants::kValueAttribute, value);
}

std::string RPCClientInstruction::GetOutput() const
{
  return Property<std::string>(domainconstants::kOutputVariableNameAttribute);
}

void RPCClientInstruction::SetOutput(const std::string &value)
{
  SetProperty(domainconstants::kOutputVariableNameAttribute, value);
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
  SetProperty(domainconstants::kCommandAttribute, value);
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
  SetProperty(domainconstants::kMessageAttribute, value);
}

std::string LogInstructionItem::GetInput() const
{
  return Property<std::string>(domainconstants::kInputVariableNameAttribute);
}

void LogInstructionItem::SetInput(const std::string &value)
{
  SetProperty(domainconstants::kInputVariableNameAttribute, value);
}

std::string LogInstructionItem::GetSeverity() const
{
  return Property<std::string>(domainconstants::kSeverityAttribute);
}

void LogInstructionItem::SetSeverity(const std::string &value)
{
  SetProperty(domainconstants::kSeverityAttribute, value);
}

}  // namespace sequencergui
