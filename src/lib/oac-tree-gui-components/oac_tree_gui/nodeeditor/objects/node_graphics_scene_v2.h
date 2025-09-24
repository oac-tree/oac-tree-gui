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

#ifndef OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_GRAPHICS_SCENE_V2_H_
#define OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_GRAPHICS_SCENE_V2_H_

#include <QGraphicsScene>

namespace oac_tree_gui
{

class InstructionItem;

/**
 * @brief The NodeGraphicsScene class is the main graphics scene of the node editor.
 */
class NodeGraphicsSceneV2 : public QGraphicsScene
{
  Q_OBJECT

public:
  explicit NodeGraphicsSceneV2(QObject* parent_object = nullptr);

  // std::vector<InstructionItem*> GetSelectedInstructions() const;

  // void SetSelectedInstructions(const std::vector<InstructionItem*>& to_select);

signals:
  void dropInstructionRequested(const QString& name, const QPointF& pos);

protected:
  void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
  void dropEvent(QGraphicsSceneDragDropEvent* event) override;

};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_GRAPHICS_SCENE_V2_H_
