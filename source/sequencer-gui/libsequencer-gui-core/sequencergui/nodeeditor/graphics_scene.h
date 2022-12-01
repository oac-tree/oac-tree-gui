/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_H_
#define SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_H_

#include <sequencergui/domain/sequencer_types.h>

#include <QGraphicsScene>
#include <map>

namespace mvvm
{
class ViewItem;
class ApplicationModel;
}  // namespace mvvm

namespace sequencergui
{
class ConnectableView;
class NodeController;
class NodeConnection;
class InstructionItem;
class InstructionContainerItem;
class MessageHandlerInterface;

class GraphicsScene : public QGraphicsScene
{
  Q_OBJECT

public:
  explicit GraphicsScene(QObject* parent = nullptr);
  ~GraphicsScene() override;

  void SetInstructionContainer(InstructionContainerItem* root_item);

  void SetMessageHandler(std::unique_ptr<MessageHandlerInterface> message_handler);

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
  void InstructionSelected(sequencergui::InstructionItem* selected);
  void selectionModeChangeRequest(int);

protected:
  void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
  void dropEvent(QGraphicsSceneDragDropEvent* event) override;

private:
  void onSelectionChanged();
  mvvm::ApplicationModel* GetModel();

  InstructionContainerItem* m_root_item{nullptr};
  std::unique_ptr<NodeController> m_node_controller;
  std::unique_ptr<MessageHandlerInterface> m_message_handler;
};

template <typename T>
inline std::vector<T*> GraphicsScene::GetSelectedViewItems() const
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_GRAPHICS_SCENE_H_
