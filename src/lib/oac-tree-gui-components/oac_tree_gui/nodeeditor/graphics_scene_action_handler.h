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

#ifndef OAC_TREE_GUI_NODEEDITOR_GRAPHICS_SCENE_ACTION_HANDLER_H_
#define OAC_TREE_GUI_NODEEDITOR_GRAPHICS_SCENE_ACTION_HANDLER_H_

#include <oac_tree_gui/composer/instruction_editor_context.h>
#include <oac_tree_gui/nodeeditor/i_graphics_scene_action_handler.h>

#include <sup/gui/core/message_event.h>

#include <functional>

namespace oac_tree_gui
{

/**
 * @brief The GraphicsSceneActionHandler class provides actual model change on connect/disconnect
 * request.
 */
class GraphicsSceneActionHandler : public IGraphicsSceneActionHandler
{
public:
  using send_message_t = std::function<void(const sup::gui::MessageEvent&)>;

  explicit GraphicsSceneActionHandler(mvvm::SessionItem* container_item,
                                      const send_message_t& send_message_callback);

  void ConnectPorts(const mvvm::INodePort* start_port, const mvvm::INodePort* end_port) override;

  void Disconnect(mvvm::INodeConnection* connection) override;

  void DoubleClickPort(const mvvm::INodePort* port) override;

private:
  mvvm::SessionItem* m_instruction_container{nullptr};
  send_message_t m_send_message;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_GRAPHICS_SCENE_ACTION_HANDLER_H_
