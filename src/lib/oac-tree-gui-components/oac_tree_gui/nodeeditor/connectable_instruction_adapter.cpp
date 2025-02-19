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

#include "connectable_instruction_adapter.h"

#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <mvvm/utils/container_utils.h>

#include <QColor>

namespace
{
const std::string xpos_attr = "xpos";
const std::string ypos_attr = "ypos";

}  // namespace

namespace oac_tree_gui
{
ConnectableInstructionAdapter::ConnectableInstructionAdapter(InstructionItem *instruction)
    : m_instruction(instruction)
{
}

QColor ConnectableInstructionAdapter::GetColor() const
{
  return GetBaseColor(m_instruction);
}

QString ConnectableInstructionAdapter::GetDisplayName() const
{
  // inserting space to allow word wrapping
  return QString::fromStdString(InsertSpaceAtCamelCase(m_instruction->GetDisplayName()));
}

std::vector<PortInfo> ConnectableInstructionAdapter::GetInputPorts() const
{
  return IsRoot() ? std::vector<PortInfo>({kRootPortInfo})
                  : std::vector<PortInfo>({kBasicPortInfo});
}

std::vector<PortInfo> ConnectableInstructionAdapter::GetOutputPorts() const
{
  return IsCompoundInstruction(GetInstruction()) ? std::vector<PortInfo>({kBasicPortInfo})
                                                 : std::vector<PortInfo>();
}

InstructionItem *ConnectableInstructionAdapter::GetInstruction() const
{
  return m_instruction;
}

double ConnectableInstructionAdapter::GetX() const
{
  return m_instruction->GetX();
}

void ConnectableInstructionAdapter::SetX(double value)
{
  m_instruction->SetX(value);
}

double ConnectableInstructionAdapter::GetY() const
{
  return m_instruction->GetY();
}

void ConnectableInstructionAdapter::SetY(double value)
{
  m_instruction->SetY(value);
}

bool ConnectableInstructionAdapter::IsRoot() const
{
  return m_instruction->IsRoot();
}

void ConnectableInstructionAdapter::SetXY(double x, double y)
{
  m_instruction->SetX(x);
  m_instruction->SetY(y);
}

}  // namespace oac_tree_gui
