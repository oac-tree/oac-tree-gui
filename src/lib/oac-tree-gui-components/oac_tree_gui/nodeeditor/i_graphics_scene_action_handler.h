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

#ifndef OAC_TREE_GUI_NODEEDITOR_I_GRAPHICS_SCENE_ACTION_HANDLER_H_
#define OAC_TREE_GUI_NODEEDITOR_I_GRAPHICS_SCENE_ACTION_HANDLER_H_

namespace mvvm
{
class INodePort;
class INodeConnection;
}  // namespace mvvm

namespace oac_tree_gui
{

/**
 * @brief The IGraphicsSceneActionHandler is an interface to propagate graphics scene change
 * requests to the underlying instruction model.
 */
class IGraphicsSceneActionHandler
{
public:
  IGraphicsSceneActionHandler() = default;
  virtual ~IGraphicsSceneActionHandler() = default;

  IGraphicsSceneActionHandler(const IGraphicsSceneActionHandler&) = delete;
  IGraphicsSceneActionHandler& operator=(const IGraphicsSceneActionHandler&) = delete;
  IGraphicsSceneActionHandler(IGraphicsSceneActionHandler&&) = delete;
  IGraphicsSceneActionHandler& operator=(IGraphicsSceneActionHandler&&) = delete;

  /**
   * @brief Connects two ports by moving underlying items so one become child of another.
   */
  virtual void ConnectPorts(const mvvm::INodePort* start_port, const mvvm::INodePort* end_port) = 0;

  /**
   * @brief Disconnects two ports by moving the child item to a top level container.
   */
  virtual void DisconnectConnection(mvvm::INodeConnection* connection) = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_I_GRAPHICS_SCENE_ACTION_HANDLER_H_
