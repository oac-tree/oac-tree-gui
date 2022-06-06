/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/connectableinstructionadapter.h"

#include "sequencergui/model/sequencerutils.h"
#include "sequencergui/model/standardinstructionitems.h"
#include "sequencergui/nodeeditor/sceneutils.h"

#include "mvvm/utils/container_utils.h"

#include <QColor>
#include <QDebug>
#include <algorithm>

namespace
{
const std::string xpos_attr = "xpos";
const std::string ypos_attr = "ypos";

}  // namespace

namespace sequencergui
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
  return QString::fromStdString(m_instruction->GetDisplayName());
}

//! Returns input ports of connectable view. This port is located on an instruction playing the role
//! of a child. Usually located on the top part of the view. The connection from this port
//! should go toward parent view.

std::vector<PortInfo> ConnectableInstructionAdapter::GetInputPorts() const
{
  return IsRoot() ? std::vector<PortInfo>({kRootPortInfo})
                  : std::vector<PortInfo>({kBasicPortInfo});
}

//! Returns output ports of connectable view. This port is located on an instruction playing the
//! role of a parent. Usually located on the bottom part of the view. The connections from this port
//! should go toward children.

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
  return m_instruction->SetX(value);
}

double ConnectableInstructionAdapter::GetY() const
{
  return m_instruction->GetY();
}

void ConnectableInstructionAdapter::SetY(double value)
{
  return m_instruction->SetY(value);
}

bool ConnectableInstructionAdapter::IsRoot() const
{
  return m_instruction->IsRoot();
}

}  // namespace sequencergui
