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

#include "favorite_instructions_viewmodel.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/core/exceptions.h>

#include <sup/gui/components/mime_conversion_helper.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/providers/standard_row_strategies.h>
#include <mvvm/providers/viewmodel_controller_factory.h>

#include <QDebug>
#include <QMimeData>

namespace oac_tree_gui
{

FavoriteInstructionsViewModel::FavoriteInstructionsViewModel(mvvm::ISessionModel* model,
                                                             QObject* parent_object)
    : ViewModel(parent_object)
{
  SetController(mvvm::factory::CreateController<mvvm::TopItemsStrategy, mvvm::LabelDataRowStrategy>(
      model, this));
}

int FavoriteInstructionsViewModel::rowCount(const QModelIndex& index) const
{
  // we do not want to show any children beneath top level items in the container
  return index.isValid() ? 0 : ViewModel::rowCount(index);
}

QStringList FavoriteInstructionsViewModel::mimeTypes() const
{
  return {kInstructionIdentifierMimeType, kInstructionEditorMimeType};
}

QMimeData* FavoriteInstructionsViewModel::mimeData(const QModelIndexList& indexes) const
{
  // first column contains a display name, and this will lead us to actual SessionItem being copied
  const auto first_column_indexes = GetFirstColumnIndexes(indexes);

  // ownership will be taken by QDrag operation
  return CreateItemIdentifierMimeData(first_column_indexes, kInstructionIdentifierMimeType)
      .release();
}

bool FavoriteInstructionsViewModel::canDropMimeData(const QMimeData* data, Qt::DropAction action,
                                                    int row, int column,
                                                    const QModelIndex& parent) const
{
  (void)row;
  (void)column;
  (void)parent;

  if (data == nullptr)
  {
    return false;
  }

  qDebug() << "FlatListViewModel::canDropMimeData:" << data << data->formats()
           << ", action:" << action << ", row:" << row << ", column:" << column
           << ", parent:" << parent;

  // internal move (change own list ordering)
  if (data->hasFormat(kInstructionIdentifierMimeType) && action == Qt::MoveAction)
  {
    return true;
  }

  // copy from outside (InstructionEditorViewmodel)
  if (data->hasFormat(kInstructionEditorMimeType) && action == Qt::CopyAction)
  {
    return true;
  }

  return false;
}

bool FavoriteInstructionsViewModel::dropMimeData(const QMimeData* data, Qt::DropAction action,
                                                 int row, int column, const QModelIndex& parent)
{
  if (!canDropMimeData(data, action, row, column, parent))
  {
    return false;
  }

  // row == -1 when we are dropping on top of an item
  // In this list we want to treat dropping on top of item as appending after it, need to change
  // parent.
  auto parent_item = row == -1 ? GetRootSessionItem() : GetSessionItemFromIndex(parent);

  // internal move (change own list ordering)
  if (data->hasFormat(kInstructionIdentifierMimeType) && action == Qt::MoveAction)
  {
    for (const auto& id : sup::gui::GetStringListFromMime(*data, kInstructionIdentifierMimeType))
    {
      auto item = GetRootSessionItem()->GetModel()->FindItem(id);
      if (item == nullptr)
      {
        throw RuntimeException("Item with id " + id + " not found in the model");
      }

      const auto destination_tagindex =
          GetListInternalMoveTagIndex(row, item->GetTagIndex(), parent);
      GetRootSessionItem()->GetModel()->MoveItem(item, parent_item, destination_tagindex);
    }
    return true;
  }

  // copy from outside (InstructionEditorViewmodel)
  if (data->hasFormat(kInstructionEditorMimeType) && action == Qt::CopyAction)
  {
    for (const auto& id : sup::gui::GetStringListFromMime(*data, kInstructionEditorMimeType))
    {
      auto item = GetRootSessionItem()->GetModel()->FindItem(id);
      if (item == nullptr)
      {
        throw RuntimeException("Item with id " + id + " not found in the model");
      }

      const auto destination_tagindex =
          GetListInternalMoveTagIndex(row, item->GetTagIndex(), parent);
      mvvm::utils::CopyItem(item, GetRootSessionItem()->GetModel(), parent_item,
                            destination_tagindex);
    }
    return true;
  }

  return false;
}

Qt::DropActions FavoriteInstructionsViewModel::supportedDropActions() const
{
  return Qt::MoveAction | Qt::CopyAction;
}

Qt::DropActions FavoriteInstructionsViewModel::supportedDragActions() const
{
  return Qt::MoveAction | Qt::CopyAction;
}

Qt::ItemFlags FavoriteInstructionsViewModel::flags(const QModelIndex& index) const
{
  auto default_flags = ViewModel::flags(index);
  if (index.isValid())
  {
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | default_flags;
  }

  // invalid item (root) can receive drops for appending at the end
  return Qt::ItemIsDropEnabled | default_flags;
}

}  // namespace oac_tree_gui
