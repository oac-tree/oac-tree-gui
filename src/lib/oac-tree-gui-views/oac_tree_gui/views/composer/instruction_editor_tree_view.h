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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_EITOR_TREE_VIEW_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_EITOR_TREE_VIEW_H_

#include <QTreeView>

namespace oac_tree_gui
{

/**
 * @brief The InstructionEditorTreeView class is a main instruction tree of instruction editor.
 *
 * It's main purpose is force MoveAction during drop operation.
 */
class InstructionEditorTreeView : public QTreeView
{
  Q_OBJECT

public:
  explicit InstructionEditorTreeView(QWidget* parent = nullptr);

protected:
  void dropEvent(QDropEvent* event) override;

};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_EITOR_TREE_VIEW_H_
