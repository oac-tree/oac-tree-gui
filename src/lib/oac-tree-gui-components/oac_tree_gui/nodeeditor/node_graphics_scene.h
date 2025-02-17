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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_NODEEDITOR_NODE_GRAPHICS_SCENE_H_
#define OAC_TREE_GUI_NODEEDITOR_NODE_GRAPHICS_SCENE_H_

#include "sup/gui/core/message_handler_interface.h"

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

  template <typename T>
  std::vector<T*> GetSelectedViewItems() const;

  void SetSelectedInstructions(const std::vector<InstructionItem*>& to_select);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  void onConnectionRequest(ConnectableView* child_view, ConnectableView* parent_view);

signals:
  void InstructionSelected(oac_tree_gui::InstructionItem* selected);
  void selectionModeChangeRequest(int);

protected:
  void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
  void dropEvent(QGraphicsSceneDragDropEvent* event) override;

private:
  void onSelectionChanged();
  mvvm::ApplicationModel* GetModel();
  InstructionEditorContext CreateContext();

  InstructionContainerItem* m_root_item{nullptr};
  std::unique_ptr<NodeConnectionController> m_node_controller;
  std::function<void(const sup::gui::MessageEvent&)> m_send_message_callback;
  std::unique_ptr<NodeGraphicsSceneActionHandler> m_action_handler;
};

template <typename T>
inline std::vector<T*> NodeGraphicsScene::GetSelectedViewItems() const
{
  std::vector<T*> result;
  for (auto item : selectedItems())
  {
    if (auto casted = dynamic_cast<T*>(item); casted)
    {
      result.push_back(casted);
    }
  }
  return result;
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_NODE_GRAPHICS_SCENE_H_
