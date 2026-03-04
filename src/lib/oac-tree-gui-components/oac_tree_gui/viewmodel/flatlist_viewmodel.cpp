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

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>

#include <QDebug>
#include <QMimeData>
#include <algorithm>

namespace oac_tree_gui
{

FlatListViewModel::FlatListViewModel(const QStringList& expected_mime_types, QObject* parent_object)
    : ViewModel(parent_object), m_expected_mime_types(expected_mime_types)
{
}

int FlatListViewModel::rowCount(const QModelIndex& index) const
{
  // we do not want to show any children beneath top level items in the container
  return index.isValid() ? 0 : ViewModel::rowCount(index);
}

QStringList FlatListViewModel::mimeTypes() const
{
  return m_expected_mime_types;
}

QMimeData* FlatListViewModel::mimeData(const QModelIndexList& indexes) const
{
  // first column contains a display name, and this will lead us to actual SessionItem being copied
  const auto first_column_indexes = GetFirstColumnIndexes(indexes);

  // ownership will be taken by QDrag operation
  return CreateItemIdentifierMimeData(first_column_indexes, GetPrimaryMimeType()).release();
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

  auto [target_item, drop_row_indicator] = GetDropTarget(row, parent);
  auto parent_item = const_cast<mvvm::SessionItem*>(target_item);
  // drop of object corresponding to internal move (from the same view model or same type of view
  // model, i.e. InstructionEditorViewModel)
  if (HandleDropItemIdentifierMimeData(*data, GetPrimaryMimeType(), action, drop_row_indicator,
                                       *parent_item))
  {
    return true;
  }

  return true;
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

std::pair<const mvvm::SessionItem*, int32_t> FlatListViewModel::GetDropTarget(
    int32_t row, const QModelIndex& parent) const
{
  // row == -1 is when we are dropping on top of an item
  // row == -1, QModelIndex(-1, -1) is when we are dropping on empty space after the last item or on
  // empty viewport

  // In this list we want to treat dropping on top of item as appending before it, need to change
  // parent.

  auto parent_item = row == -1 ? GetRootSessionItem() : GetSessionItemFromIndex(parent);

  std::int32_t drop_row_indicator = row;
  if (parent.isValid() && drop_row_indicator == -1)
  {
    drop_row_indicator = parent.row();
  }

  return std::make_pair(parent_item, drop_row_indicator);
}

QString FlatListViewModel::GetPrimaryMimeType() const
{
  // by default drag operation will create mime object corresponding to the first type in the list
  return m_expected_mime_types.empty() ? QString() : m_expected_mime_types.front();
}

}  // namespace oac_tree_gui
