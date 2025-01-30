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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_TOOLKIT_TREE_VIEW_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_TOOLKIT_TREE_VIEW_H_

#include <QTreeView>

namespace oac_tree_gui
{

/**
 * @brief The InsructionToolKitTreeView class is a tree view with instruction groups to
 * drag-and-drop on the graphics scene.
 *
 * Its only purpose is to tweak the drag-and-drop indicator.
 */
class InsructionToolKitTreeView : public QTreeView
{
  Q_OBJECT
public:
  explicit InsructionToolKitTreeView(QWidget* parent_widget = nullptr);

signals:
  void InstructionDoubleClicked(const QString& name);

protected:
  void startDrag(Qt::DropActions supported_actions) override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_TOOLKIT_TREE_VIEW_H_
