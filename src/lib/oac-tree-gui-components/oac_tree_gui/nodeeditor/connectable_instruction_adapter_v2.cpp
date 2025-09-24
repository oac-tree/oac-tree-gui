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

#include "connectable_instruction_adapter_v2.h"

#include "scene_utils.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>

namespace oac_tree_gui
{

namespace
{

constexpr std::string_view kDefaultPortType = "kDefaultPortType";

}  // namespace

ConnectableInstructionAdapterV2::ConnectableInstructionAdapterV2(InstructionItem *instruction)
    : m_instruction(instruction)
{
  if (!m_instruction)
  {
    throw RuntimeException("Instruciton item is not set");
  }
}

mvvm::SessionItem *ConnectableInstructionAdapterV2::GetItem() const
{
  return m_instruction;
}

QString ConnectableInstructionAdapterV2::GetDisplayName() const
{
  return QString::fromStdString(m_instruction->GetDisplayName());
}

QColor ConnectableInstructionAdapterV2::GetBaseColor() const
{
  return ::oac_tree_gui::GetBaseColor(m_instruction);
}

double ConnectableInstructionAdapterV2::GetX() const
{
  return m_instruction->GetX();
}

void ConnectableInstructionAdapterV2::SetX(double value)
{
  m_instruction->SetX(value);
}

double ConnectableInstructionAdapterV2::GetY() const
{
  return m_instruction->GetY();
}

void ConnectableInstructionAdapterV2::SetY(double value)
{
  m_instruction->SetY(value);
}

std::vector<mvvm::PortInfo> ConnectableInstructionAdapterV2::GetPortInfos() const
{
  const mvvm::PortInfo kInputPort(mvvm::PortDirection::kInput, std::string(kDefaultPortType), "",
                                  GetInstructionItem());

  const mvvm::PortInfo kOutputPort(mvvm::PortDirection::kOutput, std::string(kDefaultPortType), "",
                                   GetInstructionItem());

  return IsCompoundInstruction(GetInstructionItem())
             ? std::vector<mvvm::PortInfo>({kInputPort, kOutputPort})
             : std::vector<mvvm::PortInfo>({kOutputPort});
}

InstructionItem *ConnectableInstructionAdapterV2::GetInstructionItem() const
{
  return m_instruction;
}

}  // namespace oac_tree_gui
