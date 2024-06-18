/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_editor_viewmodel.h"

#include "drag_and_drop_helper.h"

#include <sequencergui/viewmodel/custom_row_strategies.h>

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/validate_utils.h>
#include <mvvm/viewmodel/standard_children_strategies.h>

#include <QMimeData>

namespace sequencergui
{

InstructionEditorViewModel::InstructionEditorViewModel(mvvm::ISessionModel *model,
                                                       QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<mvvm::TopItemsStrategy, InstructionEditorRowStrategy>(model,
                                                                                            this));
}

Qt::ItemFlags InstructionEditorViewModel::flags(const QModelIndex &index) const
{
  auto default_flags = ViewModel::flags(index);
  if (index.isValid())
  {
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | default_flags;
  }

  return Qt::ItemIsDropEnabled | default_flags;
}

QMimeData *InstructionEditorViewModel::mimeData(const QModelIndexList &index_list) const
{
  // ownership will be taken by QDrag operation
  return CreateInstructionMoveMimeData(index_list).release();
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
[4]  --------------    row_col=( 2,  0)     QModelIndex(-1, -1)    <-- problematic
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

bool InstructionEditorViewModel::canDropMimeData(const QMimeData *data, Qt::DropAction action,
                                                 int row, int column,
                                                 const QModelIndex &parent) const
{
  if (!data)
  {
    return false;
  }

  auto parent_item = GetSessionItemFromIndex(parent);
  if (!parent_item)
  {
    return false;
  }

  if (data->hasFormat(kInstructionMoveMimeType))
  {
    for (const auto &id : GetIdentifiersToMove(data))
    {
      auto item = GetRootSessionItem()->GetModel()->FindItem(id);
      auto pos = GetInternalMoveTagIndex(*item, *parent_item, row);
      if (!mvvm::utils::CanMoveItem(item, parent_item, pos).first)
      {
        return false;
      }
    }
    return true;
  }

  if (data->hasFormat(kNewInstructionMimeType))
  {
    auto drop_type = GetNewInstructionType(data);
    if (mvvm::utils::CanInsertType(drop_type, parent_item, GetDropTagIndex(row)).first)
    {
      return true;
    }
  }

  return false;
}

bool InstructionEditorViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row,
                                              int column, const QModelIndex &parent)
{
  if (!canDropMimeData(data, action, row, column, parent))
  {
    return false;
  }

  auto parent_item = GetSessionItemFromIndex(parent);

  if (data->hasFormat(kInstructionMoveMimeType))
  {
    for (const auto &id : GetIdentifiersToMove(data))
    {
      auto item = GetRootSessionItem()->GetModel()->FindItem(id);
      auto pos = GetInternalMoveTagIndex(*item, *parent_item, row);

      GetRootSessionItem()->GetModel()->MoveItem(item, parent_item, pos);
    }
    return true;
  }

  if (data->hasFormat(kNewInstructionMimeType))
  {
    if (auto drop_type = GetNewInstructionType(data); !drop_type.empty())
    {
      DropInstruction(drop_type, parent_item, GetDropTagIndex(row));
    }
    return true;
  }

  return false;
}

int InstructionEditorViewModel::columnCount(const QModelIndex &parent) const
{
  return 2;  // Name, Type
}

QStringList InstructionEditorViewModel::mimeTypes() const
{
  return {kInstructionMoveMimeType, kNewInstructionMimeType};
}

}  // namespace sequencergui
