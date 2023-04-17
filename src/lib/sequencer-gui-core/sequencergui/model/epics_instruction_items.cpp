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

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/item_constants.h>

#include <mvvm/utils/string_utils.h>

#include <sup/sequencer/instruction.h>

namespace sequencergui
{

// ----------------------------------------------------------------------------
// ChannelAccessReadInstructionItem
// ----------------------------------------------------------------------------

ChannelAccessReadInstructionItem::ChannelAccessReadInstructionItem() : InstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> ChannelAccessReadInstructionItem::Clone(
    bool make_unique_id) const
{
  return std::make_unique<ChannelAccessReadInstructionItem>(*this, make_unique_id);
}

std::string ChannelAccessReadInstructionItem::GetDomainType() const
{
  return domainconstants::kChannelAccessReadInstructionType;
}

void ChannelAccessReadInstructionItem::InitFromDomainImpl(const instruction_t *instruction) {}

void ChannelAccessReadInstructionItem::SetupDomainImpl(instruction_t *instruction) const {}

// ----------------------------------------------------------------------------
// ChannelAccessWriteInstructionItem
// ----------------------------------------------------------------------------

ChannelAccessWriteInstructionItem::ChannelAccessWriteInstructionItem() : InstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> ChannelAccessWriteInstructionItem::Clone(
    bool make_unique_id) const
{
  return std::make_unique<ChannelAccessWriteInstructionItem>(*this, make_unique_id);
}

std::string ChannelAccessWriteInstructionItem::GetDomainType() const
{
  return domainconstants::kChannelAccessWriteInstructionType;
}

void ChannelAccessWriteInstructionItem::InitFromDomainImpl(const instruction_t *instruction) {}

void ChannelAccessWriteInstructionItem::SetupDomainImpl(instruction_t *instruction) const {}

// ----------------------------------------------------------------------------
// PVAccessReadInstructionItem
// ----------------------------------------------------------------------------

PVAccessReadInstructionItem::PVAccessReadInstructionItem() : InstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> PVAccessReadInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<PVAccessReadInstructionItem>(*this, make_unique_id);
}

std::string PVAccessReadInstructionItem::GetDomainType() const
{
  return domainconstants::kPVAccessReadInstructionType;
}

void PVAccessReadInstructionItem::InitFromDomainImpl(const instruction_t *instruction) {}

void PVAccessReadInstructionItem::SetupDomainImpl(instruction_t *instruction) const {}

// ----------------------------------------------------------------------------
// PVAccessWriteInstructionItem
// ----------------------------------------------------------------------------

PVAccessWriteInstructionItem::PVAccessWriteInstructionItem() : InstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> PVAccessWriteInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<PVAccessWriteInstructionItem>(*this, make_unique_id);
}

std::string PVAccessWriteInstructionItem::GetDomainType() const
{
  return domainconstants::kPVAccessWriteInstructionType;
}

void PVAccessWriteInstructionItem::InitFromDomainImpl(const instruction_t *instruction) {}

void PVAccessWriteInstructionItem::SetupDomainImpl(instruction_t *instruction) const {}

// ----------------------------------------------------------------------------
// RPCClientInstruction
// ----------------------------------------------------------------------------

RPCClientInstruction::RPCClientInstruction() : InstructionItem(Type) {}

std::unique_ptr<mvvm::SessionItem> RPCClientInstruction::Clone(bool make_unique_id) const
{
  return std::make_unique<RPCClientInstruction>(*this, make_unique_id);
}

std::string RPCClientInstruction::GetDomainType() const
{
  return domainconstants::kRPCClientInstructionType;
}

void RPCClientInstruction::InitFromDomainImpl(const instruction_t *instruction) {}

void RPCClientInstruction::SetupDomainImpl(instruction_t *instruction) const {}

}  // namespace sequencergui
