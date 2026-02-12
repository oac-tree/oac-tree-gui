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

#include "drag_and_drop_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/transform/transform_from_domain.h>

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/session_model.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/providers/qtcore_helper.h>
#include <mvvm/providers/viewmodel_utils.h>
#include <mvvm/utils/container_utils.h>

#include <QMimeData>

namespace oac_tree_gui
{

std::vector<std::string> GetStringListFromMime(const QMimeData* mime_data, const QString& mime_type)
{
  if ((mime_data == nullptr) || !mime_data->hasFormat(mime_type))
  {
    return {};
  }
  const auto string_list = mvvm::utils::GetStringList(mime_data->data(mime_type));
  return mvvm::utils::GetStdStringVector(string_list);
}

std::unique_ptr<QMimeData> CreateItemIdentifierMimeData(const QModelIndexList& indexes,
                                                        const QString& mime_type)
{
  auto items = mvvm::utils::ItemsFromIndex(indexes);
  auto unique_items = mvvm::utils::UniqueWithOrder(items);

  QStringList identifiers;
  identifiers.reserve(static_cast<qsizetype>(unique_items.size()));
  for (const auto& item : unique_items)
  {
    identifiers.push_back(QString::fromStdString(item->GetIdentifier()));
  }

  auto result = std::make_unique<QMimeData>();
  // saving identifiers in mime data
  result->setData(mime_type, mvvm::utils::GetByteArray(identifiers));
  return result;
}

std::unique_ptr<QMimeData> CreateInstructionMoveMimeData(const QModelIndexList& indexes)
{
  // For the moment we do not support drag and move operation when more than one row is selected.
  // Also we expect that QTreeView is in QAbstractItemView::ExtendedSelection mode.

  // Drag operation in a tree view looking at InstructionEditorViewModel will generate indexes
  // corresponding to two items (display name and custom name). We expect that the first one (the
  // display name of the instruction) will point us to the instruction pointer itself.

  if (indexes.size() != 2)
  {
    return {};
  }

  auto result = std::make_unique<QMimeData>();

  auto about_to_move_item = mvvm::utils::ItemsFromIndex(indexes).at(0);

  // saving identifier in mime data
  const QStringList identifiers({QString::fromStdString(about_to_move_item->GetIdentifier())});
  result->setData(kInstructionMoveMimeType, mvvm::utils::GetByteArray(identifiers));

  return result;
}

std::unique_ptr<QMimeData> CreateNewInstructionMimeData(const QString& name)
{
  if (name.isEmpty())
  {
    return {};
  }

  auto result = std::make_unique<QMimeData>();
  result->setData(kNewInstructionMimeType, mvvm::utils::GetByteArray({name}));
  return result;
}

std::string GetNewInstructionType(const QMimeData* mime_data)
{
  if ((mime_data == nullptr) || !mime_data->hasFormat(kNewInstructionMimeType))
  {
    return {};
  }

  auto binary_data = mime_data->data(kNewInstructionMimeType);
  auto list = mvvm::utils::GetStringList(binary_data);
  return list.empty() ? std::string() : list.front().toStdString();
}

mvvm::TagIndex GetInternalMoveTagIndex(const mvvm::SessionItem& item,
                                       const mvvm::SessionItem& parent,
                                       std::int32_t drop_indicator_row)
{
  if (drop_indicator_row < 0)
  {
    // mouse is hovered on top of another item
    return mvvm::TagIndex::First();
  }

  // mouse is hovered between two other items

  // ----  drop_indicator_row = 0
  // item0
  // ----- drop_indicator_row = 1
  // item1

  if (item.GetParent() == &parent)
  {
    // if item is moved inside the same parent we have to shift by one, to insert in proper place
    auto current_tag_index = item.GetTagIndex();
    if (current_tag_index.GetIndex() < static_cast<std::size_t>(drop_indicator_row))
    {
      // if item is moved toward larger indices
      if (drop_indicator_row > 0)
      {
        return mvvm::TagIndex::Default(static_cast<std::size_t>(drop_indicator_row - 1));
      }
    }
  }

  return mvvm::TagIndex::Default(static_cast<std::size_t>(drop_indicator_row));
}

mvvm::TagIndex GetDropTagIndex(std::int32_t drop_indicator_row)
{
  if (drop_indicator_row < 0)
  {
    // mouse is hovered on top of another item
    return mvvm::TagIndex::First();
  }

  return mvvm::TagIndex::Default(static_cast<std::size_t>(drop_indicator_row));
}

std::int32_t GetListInternalMoveIndex(std::int32_t drop_indicator_row, int32_t source_index,
                                      const QModelIndex& parent)
{
  // This is what canDropMimeData reports when we drag an item
  //
  // [0]  ----------  row_col=( 0,  0)  QModelIndex(-1, -1)
  // [1]  procedure0  row_col=(-1, -1)  QModelIndex(0, 0)
  // [2]  ----------  row_col=( 1,  0)  QModelIndex(-1, -1)
  // [3]  procedure1  row_col=(-1, -1)  QModelIndex(1, 0)
  // [4]  ----------  row_col=( 2,  0)  QModelIndex(-1, -1)
  // [5]  procedure2  row_col=(-1, -1)  QModelIndex(2, 0)
  // [6]  ----------  row_col=( 3,  0)  QModelIndex(-1, -1)

  std::int32_t destination_index{-1};
  if (drop_indicator_row != -1)
  {
    // dropped between items at specific position
    destination_index = drop_indicator_row;
    if (source_index < destination_index)
    {
      destination_index--;
    }
  }
  else if (parent.isValid())
  {
    // dropped on an item
    destination_index = parent.row();
  }
  else
  {
    // dropped on viewport
    return kViewportDropIndex;
  }

  return destination_index;
}

mvvm::TagIndex GetListInternalMoveTagIndex(int32_t drop_indicator_row,
                                           const mvvm::TagIndex& source_tag_index,
                                           const QModelIndex& parent)
{
  const auto source_tag = source_tag_index.GetTag();
  auto destination_index =
      GetListInternalMoveIndex(drop_indicator_row, source_tag_index.GetIndex(), parent);

  if (destination_index == kViewportDropIndex)
  {
    return mvvm::TagIndex::Append(source_tag);
  }

  const mvvm::TagIndex target_index =
      mvvm::TagIndex{source_tag, static_cast<std::size_t>(destination_index)};

  // if item is moved inside the same parent we have to shift by one, to insert in proper place
  // FIXME consider moving this logic to GetListInternalMoveIndex
  return (source_tag_index.GetIndex() < target_index.GetIndex()) ? target_index.Next()
                                                                 : target_index;
}

}  // namespace oac_tree_gui
