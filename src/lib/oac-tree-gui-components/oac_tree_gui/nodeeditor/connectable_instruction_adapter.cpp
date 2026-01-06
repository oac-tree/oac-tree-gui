/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "connectable_instruction_adapter.h"

#include "scene_utils.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/style/graphics_scene_style.h>
#include <oac_tree_gui/style/style_helper.h>

#include <mvvm/nodeeditor/node_editor_helper.h>
#include <mvvm/nodeeditor/node_editor_types.h>

namespace oac_tree_gui
{

namespace
{

constexpr std::string_view kDefaultPortType = "kDefaultPortType";

/**
 * @brief Updates operation states from instruction status.
 */
void UpdateOperationStateFromInstructionStatus(InstructionStatus status,
                                               mvvm::NodeOperationStates& states)
{
  static const std::map<InstructionStatus, mvvm::NodeOperationState> state_map = {
      {InstructionStatus::kNotStarted, mvvm::NodeOperationState::kNotStarted},
      {InstructionStatus::kNotFinished, mvvm::NodeOperationState::kNotFinished},
      {InstructionStatus::kRunning, mvvm::NodeOperationState::kRunning},
      {InstructionStatus::kSuccess, mvvm::NodeOperationState::kSuccess},
      {InstructionStatus::kFailure, mvvm::NodeOperationState::kFailure}};

  auto iter = state_map.find(status);
  if (iter != state_map.end())
  {
    (void)states.Set(iter->second);
  }
}

}  // namespace

ConnectableInstructionAdapter::ConnectableInstructionAdapter(InstructionItem* instruction)
    : m_instruction(instruction)
{
  if (m_instruction == nullptr)
  {
    throw RuntimeException("Instruction item is not set");
  }
}

mvvm::SessionItem* ConnectableInstructionAdapter::GetItem() const
{
  return m_instruction;
}

QString ConnectableInstructionAdapter::GetDisplayName() const
{
  return QString::fromStdString(m_instruction->GetDisplayName());
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

std::vector<mvvm::PortInfo> ConnectableInstructionAdapter::GetPortInfos() const
{
  const mvvm::PortInfo kInputPort(mvvm::PortDirection::kInput, std::string(kDefaultPortType), "",
                                  GetInstructionItem());

  const mvvm::PortInfo kOutputPort(mvvm::PortDirection::kOutput, std::string(kDefaultPortType), "",
                                   GetInstructionItem());

  return IsCompoundInstruction(GetInstructionItem())
             ? std::vector<mvvm::PortInfo>({kInputPort, kOutputPort})
             : std::vector<mvvm::PortInfo>({kOutputPort});
}

InstructionItem* ConnectableInstructionAdapter::GetInstructionItem() const
{
  return m_instruction;
}

mvvm::NodeOperationStates ConnectableInstructionAdapter::GetOperationStates() const
{
  mvvm::NodeOperationStates result;
  if (!m_instruction->GetInstructions().empty() && IsCollapsed(*m_instruction))
  {
    (void)result.Set(mvvm::NodeOperationState::kCollapsedChildren);
  }

  UpdateOperationStateFromInstructionStatus(m_instruction->GetStatus(), result);

  return result;
}

void ConnectableInstructionAdapter::ProcessEvent(const mvvm::node_event_t& event) const
{
  (void)event;
}

mvvm::ConnectableShapeStyle ConnectableInstructionAdapter::GetStyle() const
{
  const static auto kSceneStyle = CreateStyleFromResource<style::GraphicsSceneStyle>();

  // adjustung shape style using scene settings
  auto result = mvvm::CreateDefaultConnectableShapeStyle();
  result.base_color = ::oac_tree_gui::GetBaseColor(m_instruction);
  result.shadow_enabled = kSceneStyle.shadow_enabled;
  return result;
}

}  // namespace oac_tree_gui
