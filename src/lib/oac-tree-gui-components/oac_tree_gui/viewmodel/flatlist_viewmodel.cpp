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

#include "flatlist_viewmodel.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/core/exceptions.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/validate_utils.h>

#include <QDebug>
#include <QMimeData>
#include <algorithm>

namespace oac_tree_gui
{

FlatListViewModel::FlatListViewModel(const QString& expected_mime_type, QObject* parent_object)
    : ViewModel(parent_object), m_expected_mime_type(expected_mime_type)
{
}

int FlatListViewModel::rowCount(const QModelIndex& index) const
{
  // we do not want to show any children beneath top level items in the container
  return index.isValid() ? 0 : ViewModel::rowCount(index);
}

QStringList FlatListViewModel::mimeTypes() const
{
  return {m_expected_mime_type};
}

QMimeData* FlatListViewModel::mimeData(const QModelIndexList& indexes) const
{
  // we assume that first column contains a display name, and this will lead us to actual
  // SessionItem being copied
  QModelIndexList first_column_indexes;
  for (const auto& index : indexes)
  {
    if (index.column() == 0)
    {
      first_column_indexes.append(index);
    }
  }

  // ownership will be taken by QDrag operation
  return CreateItemIdentifierMimeData(first_column_indexes, m_expected_mime_type).release();
}

bool FlatListViewModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row,
                                        int column, const QModelIndex& parent) const
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

  if (!(action & supportedDropActions()))
  {
    return false;
  }

  const QStringList model_types = mimeTypes();

  return std::any_of(model_types.cbegin(), model_types.cend(),
                     [data](const QString& mime_type) { return data->hasFormat(mime_type); });
}

bool FlatListViewModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row,
                                     int column, const QModelIndex& parent)
{
  if (!canDropMimeData(data, action, row, column, parent))
  {
    return false;
  }

  // row == -1 when we are dropping on top of an item
  // In this list we want to treat dropping on top of item as appending after it, need to change
  // parent.
  auto parent_item = row == -1 ? GetRootSessionItem() : GetSessionItemFromIndex(parent);

  if (data->hasFormat(m_expected_mime_type))
  {
    for (const auto& id : GetStringListFromMime(data, m_expected_mime_type))
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

  return false;
}

Qt::DropActions FlatListViewModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

Qt::DropActions FlatListViewModel::supportedDragActions() const
{
  return Qt::MoveAction | Qt::CopyAction;
}

Qt::ItemFlags FlatListViewModel::flags(const QModelIndex& index) const
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
