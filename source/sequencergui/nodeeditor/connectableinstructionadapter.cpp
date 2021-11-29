/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/nodeeditor/connectableinstructionadapter.h"

#include "sequencergui/model/instructionitems.h"

#include <QColor>
#include <QDebug>

namespace
{
const std::string xpos_attr = "xpos";
const std::string ypos_attr = "ypos";
}  // namespace

namespace sequi
{
ConnectableInstructionAdapter::ConnectableInstructionAdapter(InstructionItem *instruction) : m_instruction(instruction) {}

QColor ConnectableInstructionAdapter::GetColor() const
{
  return QColor(Qt::lightGray);
}

QString ConnectableInstructionAdapter::GetDisplayName() const
{
  return QString::fromStdString(m_instruction->GetDisplayName());
}

std::vector<PortInfo> ConnectableInstructionAdapter::GetInputPorts() const
{
  return {kBasicChildPortInfo};
}

std::vector<PortInfo> ConnectableInstructionAdapter::GetOutputPorts() const
{
  return {kBasicChildPortInfo};
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

}  // namespace sequi
