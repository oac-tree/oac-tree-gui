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

#include "graphics_scene_action_handler.h"

namespace oac_tree_gui
{

GraphicsSceneActionHandler::GraphicsSceneActionHandler(const send_message_t &send_message_callback)
    : m_send_message(send_message_callback)
{
}

void GraphicsSceneActionHandler::ConnectPorts(const mvvm::INodePort *start_port,
                                              const mvvm::INodePort *end_port)
{
  (void)start_port;
  (void)end_port;
}

void GraphicsSceneActionHandler::DisconnectConnection(mvvm::INodeConnection *connection)
{
  (void)connection;
}

}  // namespace oac_tree_gui
