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

#include "graphics_scene_action_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>

#include <mvvm/nodeeditor/i_node_connection.h>
#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/nodeeditor/node_editor_helper.h>

namespace oac_tree_gui
{

GraphicsSceneActionHandler::GraphicsSceneActionHandler(mvvm::SessionItem* container_item,
                                                       const send_message_t& send_message_callback)
    : m_instruction_container(container_item), m_send_message(send_message_callback)
{
  if (m_instruction_container == nullptr)
  {
    throw RuntimeException("Instruction container is not set");
  }

  if (!m_send_message)
  {
    throw RuntimeException("Message callback is not set");
  }
}

void GraphicsSceneActionHandler::ConnectPorts(const mvvm::INodePort* start_port,
                                              const mvvm::INodePort* end_port)
{
  try
  {
    auto succeeded = mvvm::ConnectPorts(start_port, end_port);
    (void)succeeded;
    // In current implementation ConnectPorts returns false if ports are absent, or incopmpatible.
    // The throw will happen if ports are compatible, but item doesn't allow connection due to model
    // constraints (i.e. Repeat can have only one child).
  }
  catch (const std::exception& ex)
  {
    m_send_message(sup::gui::CreateInvalidOperationMessage("Can't connect ports",
                                                           "Failed to connect ports", ex.what()));
  }
}

void GraphicsSceneActionHandler::Disconnect(mvvm::INodeConnection* connection)
{
  if (connection == nullptr)
  {
    return;
  }

  try
  {
    auto succeeded = mvvm::DisconnectPortsAndMove(
        connection->GetStartPort(), connection->GetEndPort(), m_instruction_container);
    (void)succeeded;
    // In current implementation DisconnectPortsAndMove returns false if ports are absent, or
    // incompatible. The throw will happen if ports are compatible, but item doesn't allow
    // disconnection due to model constraints.
  }
  catch (const std::exception& ex)
  {
    m_send_message(sup::gui::CreateInvalidOperationMessage(
        "Can't disconnect connection", "Failed to disconnect connection", ex.what()));
  }
}

void GraphicsSceneActionHandler::DoubleClickPort(const mvvm::INodePort* port)
{
  if (port->GetPortDirection() == mvvm::PortDirection::kInput)
  {
    if (auto instruction = mvvm::GetPortOrigin<InstructionItem>(port); instruction)
    {
      ToggleCollapsed(*instruction);
    }
  }
}

}  // namespace oac_tree_gui
