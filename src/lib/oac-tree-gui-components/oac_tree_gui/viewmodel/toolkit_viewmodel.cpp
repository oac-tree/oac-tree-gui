/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "toolkit_viewmodel.h"

#include <oac_tree_gui/viewmodel/drag_and_drop_helper.h>

#include <QMimeData>
#include <memory>

namespace
{

std::unique_ptr<QStandardItem> CreateItem(const std::string& name, bool drag_enabled)
{
  auto result = std::make_unique<QStandardItem>(QString::fromStdString(name));
  result->setEditable(false);
  result->setDragEnabled(drag_enabled);
  return result;
}

}  // namespace

namespace oac_tree_gui
{

ToolKitViewModel::ToolKitViewModel(QObject* parent_object) : QStandardItemModel(parent_object) {}

QMimeData* ToolKitViewModel::mimeData(const QModelIndexList& index_list) const
{
  if (!index_list.empty())
  {
    auto item = itemFromIndex(index_list.at(0));
    if (!item->isDragEnabled())
    {
      return nullptr;
    }
    auto mime_data = CreateNewInstructionMimeData(item->data(Qt::DisplayRole).toString());
    return mime_data.release();
  }

  return nullptr;
}

void ToolKitViewModel::PopulateModel(const std::vector<std::string>& object_types)
{
  clear();

  auto parent_item = invisibleRootItem();

  for (const auto& name : object_types)
  {
    parent_item->appendRow(CreateItem(name, /*drag_enabled*/ true).release());
  }
}

void ToolKitViewModel::PopulateModel(const std::vector<ObjectGroupInfo>& objects_group_info)
{
  clear();

  for (const auto& group_info : objects_group_info)
  {
    auto group_item = CreateItem(group_info.group_name, /*drag_enabled*/ false);
    auto parent_item = group_item.get();

    invisibleRootItem()->appendRow(group_item.release());
    for (const auto& name : group_info.object_names)
    {
      parent_item->appendRow(CreateItem(name, /*drag_enabled*/ true).release());
    }
  }
}

}  // namespace oac_tree_gui
