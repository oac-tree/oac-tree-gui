/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "drag_and_drop_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/transform/transform_from_domain.h>

// #include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/session_model.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/viewmodel/viewmodel_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMimeData>

namespace sequencergui
{

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
  QStringList identifiers({QString::fromStdString(about_to_move_item->GetIdentifier())});
  result->setData(kInstructionMoveMimeType, mvvm::utils::GetByteArray(identifiers));

  return result;
}

std::vector<std::string> GetIdentifiersToMove(const QMimeData* mime_data)
{
  if (!mime_data || !mime_data->hasFormat(kInstructionMoveMimeType))
  {
    return {};
  }
  auto identifiers = mvvm::utils::GetStringList(mime_data->data(kInstructionMoveMimeType));

  return mvvm::utils::GetStdStringVector(identifiers);
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
  if (!mime_data || !mime_data->hasFormat(kNewInstructionMimeType))
  {
    return {};
  }

  auto binary_data = mime_data->data(kNewInstructionMimeType);
  auto list = mvvm::utils::GetStringList(binary_data);
  return list.empty() ? std::string() : list.front().toStdString();
}

mvvm::TagIndex GetInternalMoveTagIndex(const mvvm::SessionItem& item,
                                       const mvvm::SessionItem& parent, int drop_indicator_row)
{
  if (drop_indicator_row < 0)
  {
    // mouse is hovered on top of another item
    return {"", 0};
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
    if (current_tag_index.index < drop_indicator_row)
    {
      // if item is moved toward larger indices
      return {"", drop_indicator_row - 1};
    }
  }

  return {"", drop_indicator_row};
}

mvvm::TagIndex GetDropTagIndex(int drop_indicator_row)
{
  if (drop_indicator_row < 0)
  {
    // mouse is hovered on top of another item
    return {"", 0};
  }

  return {"", drop_indicator_row};
}

InstructionItem* DropInstruction(const std::string& instruction_type, mvvm::SessionItem* parent,
                                 const mvvm::TagIndex& tag_index)
{
  InstructionItem* result{nullptr};

  if (!instruction_type.empty())
  {
    if (IsAggregateName(instruction_type))
    {
      result = InsertAggregate(instruction_type, parent, tag_index);
    }
    else
    {
      result = InsertInstruction(instruction_type, parent, tag_index);
    }
  }

  return result;
}

}  // namespace sequencergui
