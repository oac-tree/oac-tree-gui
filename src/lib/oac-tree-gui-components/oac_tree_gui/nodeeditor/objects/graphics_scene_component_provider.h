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

#ifndef OAC_TREE_GUI_NODEEDITOR_GRAPHICS_SCENE_COMPONENT_PROVIDER_H_
#define OAC_TREE_GUI_NODEEDITOR_GRAPHICS_SCENE_COMPONENT_PROVIDER_H_

#include <sup/gui/core/message_event.h>

#include <QObject>
#include <memory>

namespace mvvm
{
class NodeConnectionGuide;
class ConnectableViewModelController;
class SessionItem;
}  // namespace mvvm

class QGraphicsScene;

namespace oac_tree_gui
{

class IGraphicsSceneActionHandler;
class IInstructionEditorActionHandler;
class InstructionEditorContext;

/**
 * @brief The GraphicsSceneComponentProvider class holds all the logic of the graphics scene.
 *
 * - connection guide to paint connection between nodes
 * - view model controller to update the scene on Instruction model change
 * - scene action handler to propagate connect/disconnect requests from the scene to the model
 * - instruction editor action handler with the logic common to InstructionEditor
 */
class GraphicsSceneComponentProvider : public QObject
{
  Q_OBJECT

public:
  explicit GraphicsSceneComponentProvider(
      std::function<void(const sup::gui::MessageEvent&)> send_message_callback,
      QGraphicsScene* scene, mvvm::SessionItem* node_container);
  ~GraphicsSceneComponentProvider() override;

  void SetInstructionContainer();

  /**
   * @brief Deletes selected graphics items.
   *
   * The method handles shapes and their connections differently. The request to remove the shape
   * will lead to the removal of InstructionItem. The request to remove the connection, will lead
   * to moving children instructions from the parent to the top level container.
   */
  void OnDeleteSelected();

private:
  /**
   * @brief Creates view model controller.
   */
  std::unique_ptr<mvvm::ConnectableViewModelController> CreateViewModelController();

  /**
   * @brief Creates scene action handler.
   */
  std::unique_ptr<IGraphicsSceneActionHandler> CreateSceneActionHandler();

  InstructionEditorContext CreateContext();

  /**
   * @brief Creates instruction editor action handler.
   */
  std::unique_ptr<IInstructionEditorActionHandler> CreateInstructionEditorActionHandler();


  std::function<void(const sup::gui::MessageEvent&)> m_send_message_callback;
  QGraphicsScene* m_scene{nullptr};
  mvvm::SessionItem* m_node_container{nullptr};

  //!< paints connection in progress
  std::unique_ptr<mvvm::NodeConnectionGuide> m_connection_guide;

  //!< updates the scene on model change
  std::unique_ptr<mvvm::ConnectableViewModelController> m_viewmodel_controller;

  //!< handles connect/disconnect requests from the scene
  std::unique_ptr<IGraphicsSceneActionHandler> m_graphics_scene_action_handler;

  //!< handles instruction insert/remove logic common to InstructionEditor
  std::unique_ptr<IInstructionEditorActionHandler> m_instruction_editor_action_handler;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_GRAPHICS_SCENE_COMPONENT_PROVIDER_H_
