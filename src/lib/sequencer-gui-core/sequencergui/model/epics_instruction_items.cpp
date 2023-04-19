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

#include "epics_instruction_items.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/utils/string_utils.h>

#include <sup/sequencer/instruction.h>

namespace
{
const std::string kChannel = "kChannel";
const std::string kCommand = "kCommand";
const std::string kJsonType = "kJsonType";
const std::string kJsonValue = "kJsonValue";
const std::string kOutput = "kOutput";
const std::string kRequestVar = "kRequestVar";
const std::string kServiceName = "kServiceName";
const std::string kTimeout = "kTimeout";
const std::string kVariableName = "kVariableName";
}  // namespace

namespace sequencergui
{

// ----------------------------------------------------------------------------
// EpicsReadInstructionItem
// ----------------------------------------------------------------------------

EpicsReadInstructionItem::EpicsReadInstructionItem(const std::string &instruction_type)
    : InstructionItem(instruction_type)
{
  AddProperty(kChannel, std::string())->SetDisplayName("channel");
  AddProperty(kOutput, std::string())->SetDisplayName("Output variable");
  AddProperty(kTimeout, 1.0)->SetDisplayName("timeout");
}

std::unique_ptr<mvvm::SessionItem> EpicsReadInstructionItem::Clone(bool make_unique_id) const
{
  (void)make_unique_id;
  // This base is not intended to be used directly
  throw NotImplementedException("Clone for EpicsReadInstructionItem is not implemented");
}

void EpicsReadInstructionItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kChannelAttribute))
  {
    SetChannel(instruction->GetAttribute(domainconstants::kChannelAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kOutputAttribute))
  {
    SetOutput(instruction->GetAttribute(domainconstants::kOutputAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kTimeoutAttribute))
  {
    SetTimeout(std::stod(instruction->GetAttribute(domainconstants::kTimeoutAttribute)));
  }
}

void EpicsReadInstructionItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kChannelAttribute, GetChannel(), *instruction);
  AddNonEmptyAttribute(domainconstants::kOutputAttribute, GetOutput(), *instruction);
  AddNonEmptyAttribute(domainconstants::kTimeoutAttribute,
                       mvvm::utils::DoubleToString(GetTimeout()), *instruction);
}

std::string EpicsReadInstructionItem::GetChannel() const
{
  return Property<std::string>(kChannel);
}

void EpicsReadInstructionItem::SetChannel(const std::string &value)
{
  SetProperty(kChannel, value);
}

std::string EpicsReadInstructionItem::GetOutput() const
{
  return Property<std::string>(kOutput);
}

void EpicsReadInstructionItem::SetOutput(const std::string &value)
{
  SetProperty(kOutput, value);
}

double EpicsReadInstructionItem::GetTimeout() const
{
  return Property<double>(kTimeout);
}

void EpicsReadInstructionItem::SetTimeout(double value)
{
  SetProperty(kTimeout, value);
}

// ----------------------------------------------------------------------------
// EpicsWriteInstructionItem
// ----------------------------------------------------------------------------

EpicsWriteInstructionItem::EpicsWriteInstructionItem(const std::string &instruction_type)
    : InstructionItem(instruction_type)
{
  AddProperty(kVariableName, std::string())->SetDisplayName("Variable name");
  AddProperty(kChannel, std::string())->SetDisplayName("channel");
  AddProperty(kTimeout, 1.0)->SetDisplayName("timeout");
  AddProperty(kJsonType, std::string())->SetDisplayName("JSON type");
  AddProperty(kJsonValue, std::string())->SetDisplayName("JSON value");
}

std::unique_ptr<mvvm::SessionItem> EpicsWriteInstructionItem::Clone(bool make_unique_id) const
{
  (void)make_unique_id;
  // This base is not intended to be used directly
  throw NotImplementedException("Clone for EpicsReadInstructionItem is not implemented");
}

void EpicsWriteInstructionItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kVarNameAttribute))
  {
    SetVariableName(instruction->GetAttribute(domainconstants::kVarNameAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kChannelAttribute))
  {
    SetChannel(instruction->GetAttribute(domainconstants::kChannelAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kTimeoutAttribute))
  {
    SetTimeout(std::stod(instruction->GetAttribute(domainconstants::kTimeoutAttribute)));
  }

  if (instruction->HasAttribute(domainconstants::kTypeAttribute))
  {
    SetJsonType(instruction->GetAttribute(domainconstants::kTypeAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kValueAttribute))
  {
    SetJsonValue(instruction->GetAttribute(domainconstants::kValueAttribute));
  }
}

void EpicsWriteInstructionItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kVarNameAttribute, GetVariableName(), *instruction);
  AddNonEmptyAttribute(domainconstants::kChannelAttribute, GetChannel(), *instruction);
  AddNonEmptyAttribute(domainconstants::kTimeoutAttribute,
                       mvvm::utils::DoubleToString(GetTimeout()), *instruction);
  AddNonEmptyAttribute(domainconstants::kTypeAttribute, GetJsonType(), *instruction);
  AddNonEmptyAttribute(domainconstants::kValueAttribute, GetJsonValue(), *instruction);
}

std::string EpicsWriteInstructionItem::GetVariableName() const
{
  return Property<std::string>(kVariableName);
}

void EpicsWriteInstructionItem::SetVariableName(const std::string &value)
{
  SetProperty(kVariableName, value);
}

std::string EpicsWriteInstructionItem::GetChannel() const
{
  return Property<std::string>(kChannel);
}

void EpicsWriteInstructionItem::SetChannel(const std::string &value)
{
  SetProperty(kChannel, value);
}

double EpicsWriteInstructionItem::GetTimeout() const
{
  return Property<double>(kTimeout);
}

void EpicsWriteInstructionItem::SetTimeout(double value)
{
  SetProperty(kTimeout, value);
}

std::string EpicsWriteInstructionItem::GetJsonType() const
{
  return Property<std::string>(kJsonType);
}

void EpicsWriteInstructionItem::SetJsonType(const std::string &value)
{
  SetProperty(kJsonType, value);
}

std::string EpicsWriteInstructionItem::GetJsonValue() const
{
  return Property<std::string>(kJsonValue);
}

void EpicsWriteInstructionItem::SetJsonValue(const std::string &value)
{
  SetProperty(kJsonValue, value);
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

std::string ChannelAccessReadInstructionItem::GetDomainType() const
{
  return domainconstants::kChannelAccessReadInstructionType;
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

std::string ChannelAccessWriteInstructionItem::GetDomainType() const
{
  return domainconstants::kChannelAccessWriteInstructionType;
}

// ----------------------------------------------------------------------------
// PVAccessReadInstructionItem
// ----------------------------------------------------------------------------

PvAccessReadInstructionItem::PvAccessReadInstructionItem() : EpicsReadInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> PvAccessReadInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<PvAccessReadInstructionItem>(*this, make_unique_id);
}

std::string PvAccessReadInstructionItem::GetDomainType() const
{
  return domainconstants::kPvAccessReadInstructionType;
}

// ----------------------------------------------------------------------------
// PVAccessWriteInstructionItem
// ----------------------------------------------------------------------------

PvAccessWriteInstructionItem::PvAccessWriteInstructionItem() : EpicsWriteInstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> PvAccessWriteInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<PvAccessWriteInstructionItem>(*this, make_unique_id);
}

std::string PvAccessWriteInstructionItem::GetDomainType() const
{
  return domainconstants::kPvAccessWriteInstructionType;
}

// ----------------------------------------------------------------------------
// RPCClientInstruction
// ----------------------------------------------------------------------------

RPCClientInstruction::RPCClientInstruction() : InstructionItem(Type)
{
  AddProperty(kServiceName, std::string())->SetDisplayName("Service");
  AddProperty(kRequestVar, std::string())->SetDisplayName("Request variable");
  AddProperty(kTimeout, 1.0)->SetDisplayName("timeout");
  AddProperty(kJsonType, std::string())->SetDisplayName("JSON type");
  AddProperty(kJsonValue, std::string())->SetDisplayName("JSON value");
  AddProperty(kOutput, std::string())->SetDisplayName("Output");
}

std::unique_ptr<mvvm::SessionItem> RPCClientInstruction::Clone(bool make_unique_id) const
{
  return std::make_unique<RPCClientInstruction>(*this, make_unique_id);
}

std::string RPCClientInstruction::GetDomainType() const
{
  return domainconstants::kRPCClientInstructionType;
}

void RPCClientInstruction::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kServiceAttribute))
  {
    SetService(instruction->GetAttribute(domainconstants::kServiceAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kRequestAttribute))
  {
    SetRequestVar(instruction->GetAttribute(domainconstants::kRequestAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kTimeoutAttribute))
  {
    SetTimeout(std::stod(instruction->GetAttribute(domainconstants::kTimeoutAttribute)));
  }

  if (instruction->HasAttribute(domainconstants::kTypeAttribute))
  {
    SetJsonType(instruction->GetAttribute(domainconstants::kTypeAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kValueAttribute))
  {
    SetJsonValue(instruction->GetAttribute(domainconstants::kValueAttribute));
  }

  if (instruction->HasAttribute(domainconstants::kOutputAttribute))
  {
    SetOutput(instruction->GetAttribute(domainconstants::kOutputAttribute));
  }
}

void RPCClientInstruction::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kServiceAttribute, GetService(), *instruction);
  AddNonEmptyAttribute(domainconstants::kRequestAttribute, GetRequestVar(), *instruction);
  AddNonEmptyAttribute(domainconstants::kTimeoutAttribute,
                       mvvm::utils::DoubleToString(GetTimeout()), *instruction);
  AddNonEmptyAttribute(domainconstants::kTypeAttribute, GetJsonType(), *instruction);
  AddNonEmptyAttribute(domainconstants::kValueAttribute, GetJsonValue(), *instruction);
  AddNonEmptyAttribute(domainconstants::kOutputAttribute, GetOutput(), *instruction);
}

std::string RPCClientInstruction::GetService() const
{
  return Property<std::string>(kServiceName);
}

void RPCClientInstruction::SetService(const std::string &value)
{
  SetProperty(kServiceName, value);
}

std::string RPCClientInstruction::GetRequestVar() const
{
  return Property<std::string>(kRequestVar);
}

void RPCClientInstruction::SetRequestVar(const std::string &value)
{
  SetProperty(kRequestVar, value);
}

double RPCClientInstruction::GetTimeout() const
{
  return Property<double>(kTimeout);
}

void RPCClientInstruction::SetTimeout(double value)
{
  SetProperty(kTimeout, value);
}

std::string RPCClientInstruction::GetJsonType() const
{
  return Property<std::string>(kJsonType);
}

void RPCClientInstruction::SetJsonType(const std::string &value)
{
  SetProperty(kJsonType, value);
}

std::string RPCClientInstruction::GetJsonValue() const
{
  return Property<std::string>(kJsonValue);
}

void RPCClientInstruction::SetJsonValue(const std::string &value)
{
  SetProperty(kJsonValue, value);
}

std::string RPCClientInstruction::GetOutput() const
{
  return Property<std::string>(kOutput);
}

void RPCClientInstruction::SetOutput(const std::string &value)
{
  SetProperty(kOutput, value);
}

// ----------------------------------------------------------------------------
// SystemCallInstructionItem
// ----------------------------------------------------------------------------

SystemCallInstructionItem::SystemCallInstructionItem() : InstructionItem(Type)
{
  AddProperty(kCommand, std::string())->SetDisplayName("Command");
}

std::unique_ptr<mvvm::SessionItem> SystemCallInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<SystemCallInstructionItem>(*this, make_unique_id);
}

std::string SystemCallInstructionItem::GetDomainType() const
{
  return domainconstants::kSystemCallInstructionType;
}

void SystemCallInstructionItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (instruction->HasAttribute(domainconstants::kCommandAttribute))
  {
    SetCommand(instruction->GetAttribute(domainconstants::kCommandAttribute));
  }
}

void SystemCallInstructionItem::SetupDomainImpl(instruction_t *instruction) const
{
  AddNonEmptyAttribute(domainconstants::kCommandAttribute, GetCommand(), *instruction);
}

std::string SystemCallInstructionItem::GetCommand() const
{
  return Property<std::string>(kCommand);
}

void SystemCallInstructionItem::SetCommand(const std::string &value)
{
  SetProperty(kCommand, value);
}

}  // namespace sequencergui
