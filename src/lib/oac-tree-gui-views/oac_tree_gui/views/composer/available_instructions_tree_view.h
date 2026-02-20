/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_AVAILABLE_INSTRUCTIONS_TREE_VIEW_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_AVAILABLE_INSTRUCTIONS_TREE_VIEW_H_

#include <QTreeView>

namespace oac_tree_gui
{

/**
 * @brief The AvailableInstructionsTreeView class is a tree view with available instruction types.
 *
 * Instructions are groupped according to plugin group names. You can drag instruction name on
 * graphics scene or instruction tree, but you can't drag the group name.
 */
class AvailableInstructionsTreeView : public QTreeView
{
  Q_OBJECT
public:
  explicit AvailableInstructionsTreeView(QWidget* parent_widget = nullptr);

signals:
  void InstructionDoubleClicked(const QString& name);

protected:
  void startDrag(Qt::DropActions supported_actions) override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_AVAILABLE_INSTRUCTIONS_TREE_VIEW_H_
