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

#include "instruction_editor_viewmodel.h"

#include <oac_tree_gui/components/custom_row_strategies.h>
#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/composer/instruction_editor_drop_handler.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/providers/viewmodel_controller_factory.h>

// #include <QDebug>
#include <QMimeData>

namespace oac_tree_gui
{

InstructionEditorViewModel::InstructionEditorViewModel(mvvm::ISessionModel* model,
                                                       QObject* parent_object)
    : ViewModel(parent_object)
    , m_drop_handler(
          std::make_unique<InstructionEditorDropHandler>([this]() { return GetRootSessionItem(); }))
{
  SetController(
      mvvm::factory::CreateController<mvvm::TopItemsStrategy, InstructionEditorRowStrategy>(model,
                                                                                            this));
}

InstructionEditorViewModel::~InstructionEditorViewModel() = default;

Qt::ItemFlags InstructionEditorViewModel::flags(const QModelIndex& index) const
{
  auto default_flags = ViewModel::flags(index);
  if (index.isValid())
  {
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | default_flags;
  }

  return Qt::ItemIsDropEnabled | default_flags;
}

QStringList InstructionEditorViewModel::mimeTypes() const
{
  return {kNewInstructionMimeType, kInstructionIdentifierMimeType, kCopyInstructionMimeType};
}

QMimeData* InstructionEditorViewModel::mimeData(const QModelIndexList& index_list) const
{
  return CreateInstructionEditorMimeData(index_list).release();
}

Qt::DropActions InstructionEditorViewModel::supportedDragActions() const
{
  return Qt::MoveAction | Qt::CopyAction;
}

Qt::DropActions InstructionEditorViewModel::supportedDropActions() const
{
  return Qt::MoveAction | Qt::CopyAction;
}

/*
Here we explain parameters reported by canDropMimeData method. Suppose we have the following tree
hierarchy:

Message
Sequence
  Wait
Message

Schema below shows drop areas and reported parameters. Horizontal dashed lines appears as drop
indicators on attempt to drop between cells.

[0]  --------------    row_col=( 0,  0)     QModelIndex(-1, -1)
[1]  Message           row_col=(-1, -1)     QModelIndex(0, 0)
[2]  --------------    row_col=( 1,  0)     QModelIndex(-1, -1)
[3]  Sequence          row_col=(-1, -1)     QModelIndex(1, 0)
[4]  --------------    row_col=( 2,  0)     QModelIndex(-1, -1)
[5]     -----------    row_col=( 0,  0)     QModelIndex(1, 0)
[6]     Wait           row_col=(-1, -1)     QModelIndex(0, 0)
[7]     -----------    row_col=( 1,  0)     QModelIndex(1, 0)
[8]  --------------    row_col=( 2,  0)     QModelIndex(-1, -1)
[9]  Message           row_col=(-1, -1)     QModelIndex(2, 0)
[10] --------------    row_col=( 3,  0)     QModelIndex(-1, -1)

Area #4 behaves in a somewhat confusing manner. Its drop indicator looks almost similar to
area #5, but it reports the same drop parameters, as area #8. Thus, there is no way to distinguish
#4 and #8 on the model level. An attempt to drop an item in the area #4 will lead to the item
creation at #8.

I think this is OK and can be considered as a feature. If it will become an issue, we will need to
implement custom QTreeView, which would hide the #4 drop indicator based on mouse move coordinates.
*/

bool InstructionEditorViewModel::canDropMimeData(const QMimeData* data, Qt::DropAction action,
                                                 int row, int column,
                                                 const QModelIndex& parent) const
{
  (void)column;

  const QStringList formats = (data == nullptr) ? QStringList() : data->formats();
  // qDebug() << "InstructionEditorViewModel::canDropMimeData:" << data << formats
  //          << ", action:" << action << ", row:" << row << ", column:" << column
  //          << ", parent:" << parent;

  return m_drop_handler->CanDropMimeData(data, action, row, parent);
}

bool InstructionEditorViewModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row,
                                              int column, const QModelIndex& parent)
{
  if (!canDropMimeData(data, action, row, column, parent))
  {
    return false;
  }
  return m_drop_handler->DropMimeData(data, action, row, parent);
}

}  // namespace oac_tree_gui
