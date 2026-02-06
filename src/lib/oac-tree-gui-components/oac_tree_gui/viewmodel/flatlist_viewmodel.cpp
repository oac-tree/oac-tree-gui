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

#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/providers/standard_row_strategies.h>
#include <mvvm/providers/viewmodel_controller_factory.h>

#include <QMimeData>

namespace oac_tree_gui
{

FlatListViewModel::FlatListViewModel(mvvm::ISessionModel* model, QObject* parent_object)
    : ViewModel(parent_object)
{
  SetController(mvvm::factory::CreateController<mvvm::TopItemsStrategy, mvvm::LabelDataRowStrategy>(
      model, this));
}

int FlatListViewModel::rowCount(const QModelIndex& index) const
{
  // we do not want to show any children beneath top level items in the container
  return index.isValid() ? 0 : ViewModel::rowCount(index);
}

QStringList FlatListViewModel::mimeTypes() const
{
  return {kItemIdentifierMimeType};
}

QMimeData* FlatListViewModel::mimeData(const QModelIndexList& indexes) const
{
  // ownership will be taken by QDrag operation
  return CreateItemIdentifierMimeData(indexes, kItemIdentifierMimeType).release();
}

bool FlatListViewModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row,
                                        int column, const QModelIndex& parent) const
{
  (void)action;
  (void)row;
  (void)column;
  (void)parent;

  if (data == nullptr)
  {
    return false;
  }

  if (!data->hasFormat(kItemIdentifierMimeType))
  {
    return false;
  }

  return false;
}

bool FlatListViewModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row,
                                     int column, const QModelIndex& parent)
{
  return false;
}

Qt::DropActions FlatListViewModel::supportedDropActions() const
{
  return Qt::MoveAction | Qt::CopyAction;
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
