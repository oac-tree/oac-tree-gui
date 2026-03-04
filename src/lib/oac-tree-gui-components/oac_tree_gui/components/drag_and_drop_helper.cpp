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

#include <oac_tree_gui/composer/instruction_copy_helper.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>

#include <sup/gui/components/mime_conversion_helper.h>

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/model/validate_utils.h>
#include <mvvm/providers/qtcore_helper.h>
#include <mvvm/providers/viewmodel_utils.h>

#include <QMimeData>

namespace oac_tree_gui
{

QModelIndexList GetFirstColumnIndexes(const QModelIndexList& indexes)
{
  QModelIndexList result;
  for (const auto& index : indexes)
  {
    if (index.column() == 0)
    {
      result.push_back(index);
    }
  }
  return result;
}

std::unique_ptr<QMimeData> CreateItemIdentifierMimeData(const QModelIndexList& indexes,
                                                        const QString& mime_type)
{
  auto items = mvvm::utils::ItemsFromIndex(indexes);
  auto unique_items = mvvm::utils::UniqueWithOrder(items);
  return sup::gui::CreateItemSelectionIdentifierMimeData(mvvm::utils::MakeConst(unique_items),
                                                         mime_type);
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

std::unique_ptr<QMimeData> CreateInstructionEditorMimeData(const QModelIndexList& indexes)
{
  // first column contains a display name, and this will lead us to actual SessionItem being copied
  const auto first_column_indexes = GetFirstColumnIndexes(indexes);
  auto items = mvvm::utils::MakeConst(mvvm::utils::ItemsFromIndex(first_column_indexes));

  auto result = std::make_unique<QMimeData>();
  sup::gui::AddItemSelectionIdentifierToMimeData(items, kInstructionIdentifierMimeType, *result);
  sup::gui::AddItemSelectionCopyToMimeData(items, kCopyInstructionMimeType, *result);

  return result;
}

// Positions of drop indicator and reported parameters of canDropMimeData:
//
// [0 ]  --------------   row_col=( 0,  0)    QModelIndex(-1, -1)   Container
// [1 ]  sequence0        row_col=(-1, -1)    QModelIndex(0, 0)
// [2 ]  --------------   row_col=( 1,  0)    QModelIndex(-1, -1)   Container
// [3 ]      -----------  row_col=( 0,  0)    QModelIndex(0, 0)     Sequence
// [4 ]      Wait0        row_col=(-1, -1)    QModelIndex(0, 0)
// [5 ]      -----------  row_col=( 1,  0)    QModelIndex(0, 0)     Sequence
// [6 ]      Wait1        row_col=(-1, -1)    QModelIndex(1, 0)
// [7 ]      -----------  row_col=( 2,  0)    QModelIndex(0, 0)     Sequence
// [8 ]      Wait2        row_col=(-1, -1)    QModelIndex(2, 0)
// [9 ]      -----------  row_col=( 3,  0)    QModelIndex(0, 0)     Sequence
// [10]  --------------   row_col=( 1,  0)    QModelIndex(-1, -1)   Container
// [11]  sequence1        row_col=(-1, -1)    QModelIndex(1, 0)
// [12]  --------------   row_col=( 2,  0)    QModelIndex(-1, -1)   Container
// [13]     -----------   row_col=( 0,  0)    QModelIndex(1, 0)     Sequence
// [14]     Wait3         row_col=(-1, -1)    QModelIndex(0, 0)
// [15]     -----------   row_col=( 1,  0)    QModelIndex(1, 0)     Sequence
//
// [viewport]             row_col=(-1, -1)    QModelIndex(-1, -1)

mvvm::TagIndex GetTreeInternalMoveTagIndex(int32_t drop_indicator_row,
                                           const mvvm::SessionItem& child,
                                           const mvvm::SessionItem& parent)
{
  (void)child;
  (void)parent;

  if (drop_indicator_row < 0)
  {
    // Mouse is hovered on top of another item, we will use it as a parent and append (handled on
    // ViewModel side). This also covers the case of hovering on empty viewport area
    return mvvm::TagIndex::Append();
  }

  // our TagIndex convention for move operations coincides  with Qt's drop indicator convention
  return mvvm::TagIndex::Default(drop_indicator_row);
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

bool CanDropNewType(const QMimeData& data, Qt::DropAction action, int32_t drop_row_indicator,
                    const mvvm::SessionItem& parent)
{
  // we don't care about action for new type insertion, it will be always new object creation
  (void)action;

  if (data.hasFormat(kNewInstructionMimeType))
  {
    auto drop_type = GetNewInstructionType(&data);
    if (!mvvm::utils::GetInsertTypeErrorCode(drop_type, parent,
                                             GetDropTagIndex(drop_row_indicator)))
    {
      return true;
    }
  }

  return false;
}

bool HandleDropNewType(const QMimeData& data, Qt::DropAction action, int32_t drop_row_indicator,
                       mvvm::SessionItem& parent)
{
  // we don't care about action for new type insertion, it will be always new object creation
  (void)action;

  if (data.hasFormat(kNewInstructionMimeType))
  {
    if (auto drop_type = GetNewInstructionType(&data); !drop_type.empty())
    {
      auto new_item = CreateInstructionTree(drop_type);
      const auto drop_tag_index = GetDropTagIndex(drop_row_indicator);
      if (mvvm::utils::GetInsertTypeErrorCode(drop_type, parent, drop_tag_index))
      {
        return false;
      }

      parent.InsertItem(std::move(new_item), drop_tag_index);
    }
    return true;
  }

  return false;
}

bool CanDropItemIdentifierMimeData(const QMimeData& data, const QString& mime_type,
                                   Qt::DropAction action, int32_t drop_row_indicator,
                                   const mvvm::SessionItem& parent)
{
  if (action != Qt::MoveAction)
  {
    return false;
  }

  if (data.hasFormat(mime_type))
  {
    for (const auto& id : sup::gui::GetStringListFromMime(data, mime_type))
    {
      auto item = parent.GetModel()->FindItem(id);
      if (item == nullptr)
      {
        return false;
      }
      auto destination_tagindex = GetTreeInternalMoveTagIndex(drop_row_indicator, *item, parent);
      const auto move_info = mvvm::utils::GetMoveOperationInfo(*item, parent, destination_tagindex);
      if (move_info.error_code)
      {
        return false;
      }
    }
    return true;
  }

  return false;
}

bool HandleDropItemIdentifierMimeData(const QMimeData& data, const QString& mime_type,
                                      Qt::DropAction action, int32_t drop_row_indicator,
                                      mvvm::SessionItem& parent)
{
  if (action != Qt::MoveAction)
  {
    return false;
  }

  if (data.hasFormat(mime_type))
  {
    for (const auto& id : sup::gui::GetStringListFromMime(data, mime_type))
    {
      auto item = parent.GetModel()->FindItem(id);
      if (item == nullptr)
      {
        return false;
      }
      auto destination_tagindex = GetTreeInternalMoveTagIndex(drop_row_indicator, *item, parent);
      const auto move_info = mvvm::utils::GetMoveOperationInfo(*item, parent, destination_tagindex);
      if (move_info.error_code)
      {
        return false;
      }
      parent.GetModel()->MoveItem(item, &parent, destination_tagindex);
    }
    return true;
  }

  return false;
}

bool CanDropInstructionCopyMimeData(const QMimeData& data, Qt::DropAction action,
                                    int32_t drop_row_indicator, const mvvm::SessionItem& parent)
{
  if (action != Qt::CopyAction)
  {
    return false;
  }

  if (data.hasFormat(kCopyInstructionMimeType))
  {
    auto type_to_copy = sup::gui::GetSessionItemType(data, kCopyInstructionMimeType);
    return mvvm::utils::GetInsertTypeErrorCode(type_to_copy, parent,
                                               GetDropTagIndex(drop_row_indicator))
           == mvvm::ModelError::Success;
  }

  return false;
}

bool HandleDropInstructionCopyMimeData(const QMimeData& data, Qt::DropAction action,
                                       int32_t drop_row_indicator, mvvm::SessionItem& parent)
{
  if (action != Qt::CopyAction)
  {
    return false;
  }

  if (data.hasFormat(kCopyInstructionMimeType))
  {
    auto new_items = CreateInstructions(&data);
    auto last_tag_index = GetDropTagIndex(drop_row_indicator);
    for (auto& item : new_items)
    {
      auto result = parent.InsertItem(std::move(item), last_tag_index);
      last_tag_index = result->GetTagIndex().Next();
    }
    return true;
  }

  return false;
}

}  // namespace oac_tree_gui
