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

#ifndef OAC_TREE_GUI_NODEEDITOR_NODE_GRAPHICS_SCENE_H_
#define OAC_TREE_GUI_NODEEDITOR_NODE_GRAPHICS_SCENE_H_

#include <sup/gui/core/message_event.h>

#include <mvvm/utils/container_utils.h>
#include <oac_tree_gui/nodeeditor/graphics_scene_types.h>

#include <QGraphicsScene>

namespace mvvm
{
class ApplicationModel;
}  // namespace mvvm

namespace oac_tree_gui
{

class ConnectableView;
class NodeConnectionController;
class NodeConnection;
class InstructionItem;
class InstructionContainerItem;
class NodeGraphicsSceneActionHandler;
class InstructionEditorContext;

/**
 * @brief The NodeGraphicsScene class is the main graphics scene of the node editor.
 */
class NodeGraphicsScene : public QGraphicsScene
{
  Q_OBJECT

public:
  explicit NodeGraphicsScene(
      std::function<void(const sup::gui::MessageEvent&)> send_message_callback,
      QObject* parent_object = nullptr);
  ~NodeGraphicsScene() override;

  void SetInstructionContainer(InstructionContainerItem* root_item);

  bool HasContext();

  void ResetContext();

  std::vector<ConnectableView*> GetConnectableViews();
  ConnectableView* FindViewForInstruction(InstructionItem* instruction);

  void OnDeleteSelectedRequest();

  void disconnectConnectedViews(NodeConnection* connection);

  /**
   * @brief Returns vector of scene items casted to a specified type.
   */
  template <typename T>
  std::vector<T*> GetViewItems() const;

  /**
   * @brief Returns vector of selected items casted to a specified type.
   */
  template <typename T>
  std::vector<T*> GetSelectedViewItems() const;

  void SetSelectedInstructions(const std::vector<InstructionItem*>& to_select);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  void onConnectionRequest(ConnectableView* child_view, ConnectableView* parent_view);

  /**
   * @brief Creates a new single instruction or sub-tree at given coordinate.
   *
   * @param name The type of single instruction or the name of registered sub-tree.
   * @param ref_pos Coordinates on scene.
   */
  void DropInstructionTree(const std::string& name, const QPointF& ref_pos);

signals:
  void InstructionSelected(oac_tree_gui::InstructionItem* selected);
  void OperationModeChangeRequest(oac_tree_gui::GraphicsViewOperationMode);

protected:
  void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
  void dropEvent(QGraphicsSceneDragDropEvent* event) override;

private:
  void onSelectionChanged();
  mvvm::ApplicationModel* GetModel();
  InstructionEditorContext CreateContext();

  InstructionContainerItem* m_instruction_container{nullptr};
  std::unique_ptr<NodeConnectionController> m_node_controller;
  std::function<void(const sup::gui::MessageEvent&)> m_send_message_callback;
  std::unique_ptr<NodeGraphicsSceneActionHandler> m_action_handler;
};

template <typename T>
inline std::vector<T*> NodeGraphicsScene::GetViewItems() const
{
  return mvvm::utils::CastItems<T>(items());
}

template <typename T>
inline std::vector<T*> NodeGraphicsScene::GetSelectedViewItems() const
{
  return mvvm::utils::CastItems<T>(selectedItems());
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_NODE_GRAPHICS_SCENE_H_
