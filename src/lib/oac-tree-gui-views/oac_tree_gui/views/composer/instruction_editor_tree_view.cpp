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

#include "instruction_editor_tree_view.h"

#include <QDropEvent>

namespace oac_tree_gui
{

InstructionEditorTreeView::InstructionEditorTreeView(QWidget* parent) : QTreeView(parent) {}

void InstructionEditorTreeView::dropEvent(QDropEvent* event)
{
  if (auto widget = dynamic_cast<InstructionEditorTreeView*>(event->source()); widget)
  {
    // we handle all events coming from the same type of the widget as moveAction
    event->setDropAction(Qt::MoveAction);
  }
  else
  {
    event->setDropAction(Qt::CopyAction);
  }

  QTreeView::dropEvent(event);
}

}  // namespace oac_tree_gui
