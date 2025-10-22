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

#ifndef OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_GRAPHICS_SCENE_H_
#define OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_GRAPHICS_SCENE_H_

#include <QGraphicsScene>

namespace mvvm
{
class INodePort;
}

namespace oac_tree_gui
{

class InstructionItem;

/**
 * @brief The NodeGraphicsScene class is the main graphics scene of the node editor.
 */
class NodeGraphicsScene : public QGraphicsScene
{
  Q_OBJECT

public:
  explicit NodeGraphicsScene(QObject* parent_object = nullptr);

signals:
  void dropInstructionRequested(const QString& name, const QPointF& pos);
  void instructionDoubleClick(oac_tree_gui::InstructionItem* instruction);
  void portDoubleClick(const mvvm::INodePort* port);

protected:
  void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
  void dropEvent(QGraphicsSceneDragDropEvent* event) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_GRAPHICS_SCENE_H_
