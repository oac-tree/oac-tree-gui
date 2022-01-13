/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_GRAPHICSSCENE_H
#define SEQUENCERGUI_NODEEDITOR_GRAPHICSSCENE_H

#include "sequencergui/model/sequencer_types.h"

#include <QGraphicsScene>
#include <map>

namespace ModelView
{
class ViewItem;
}  // namespace ModelView

namespace sequencergui
{
class ConnectableView;
class NodeController;
class NodeConnection;
class InstructionItem;
class SequencerModel;
class InstructionContainerItem;

class GraphicsScene : public QGraphicsScene
{
  Q_OBJECT

public:
  GraphicsScene(QObject* parent = nullptr);
  ~GraphicsScene() override;

  void SetContext(SequencerModel* model, InstructionContainerItem* root_item);

  std::vector<ConnectableView*> GetConnectableViews();
  ConnectableView* FindViewForInstruction(InstructionItem* instruction);

  void onDeleteSelectedRequest();

  void disconnectConnectedViews(NodeConnection* connection);

  template <typename T>
  std::vector<T*> GetSelectedViewItems() const;

  void SetSelectedInstructions(const std::vector<InstructionItem *> &to_select);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

signals:
  void InstructionSelected(InstructionItem* selected);
  void selectionModeChangeRequest(int);

protected:
  void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
  void dropEvent(QGraphicsSceneDragDropEvent* event) override;

public slots:
  void onConnectionRequest(ConnectableView* childView, ConnectableView* parentView);

private slots:
  void onSelectionChanged();

private:

  SequencerModel* m_model{nullptr};
  InstructionContainerItem* m_root_item{nullptr};

  std::unique_ptr<NodeController> m_node_controller;
  qreal m_vertical_pos;
};

template <typename T>
inline std::vector<T*> GraphicsScene::GetSelectedViewItems() const
{
  std::vector<T*> result;
  for (auto item : selectedItems())
    if (auto casted = dynamic_cast<T*>(item); casted)
      result.push_back(casted);
  return result;
}

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_GRAPHICSSCENE_H
