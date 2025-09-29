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
class ConnectableShape;
class ISessionModel;
}  // namespace mvvm

class QGraphicsScene;

namespace oac_tree_gui
{

class IGraphicsSceneActionHandler;
class IInstructionEditorActionHandler;
class InstructionEditorContext;
class InstructionItem;

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
  using position_t = std::pair<double, double>;

  explicit GraphicsSceneComponentProvider(
      std::function<void(const sup::gui::MessageEvent&)> send_message_callback,
      std::function<std::string(const std::string&)> object_to_plugin_name, QGraphicsScene* scene,
      mvvm::SessionItem* instruction_container);
  ~GraphicsSceneComponentProvider() override;

  /**
   * @brief Deletes selected graphics items.
   *
   * The method handles shapes and their connections differently. The request to remove the shape
   * will lead to the removal of InstructionItem. The request to remove the connection, will lead
   * to moving children instructions from the parent to the top level container.
   */
  void OnDeleteSelected();

  /**
   * @brief Returns currently selected instructions.
   *
   * Converts all selected shapes to selected instructions.
   */
  std::vector<InstructionItem*> GetSelectedInstructions() const;

  /**
   * @brief Sets selected instructions.
   *
   * Converts instructions to shapes and selects them on the scene.
   */
  void SetSelectedInstructions(const std::vector<InstructionItem*>& to_select);

  /**
   * @brief Drops instruction on the canvas.
   */
  void DropInstruction(const std::string& item_type, const position_t& pos);

signals:
  void selectionChanged();

private:
  mvvm::ISessionModel* GetModel() const;

  /**
   * @brief Setup all necessary connections.
   */
  void SetupConnections();

  /**
   * @brief Handles scene selection change.
   */
  void OnSceneSelectionChanged();

  /**
   * @brief Creates view model controller.
   */
  std::unique_ptr<mvvm::ConnectableViewModelController> CreateViewModelController();

  /**
   * @brief Creates scene action handler.
   */
  std::unique_ptr<IGraphicsSceneActionHandler> CreateSceneActionHandler();

  /**
   * @brief Creates context necessary for instruction editor action handler.
   */
  InstructionEditorContext CreateContext();

  /**
   * @brief Creates instruction editor action handler.
   */
  std::unique_ptr<IInstructionEditorActionHandler> CreateInstructionEditorActionHandler();

  std::function<void(const sup::gui::MessageEvent&)> m_send_message_callback;
  std::function<std::string(const std::string&)> m_object_to_plugin_name;
  QGraphicsScene* m_scene{nullptr};
  mvvm::SessionItem* m_instruction_container{nullptr};

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
