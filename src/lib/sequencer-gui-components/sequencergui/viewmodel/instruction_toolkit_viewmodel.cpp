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

#include "instruction_toolkit_viewmodel.h"

#include <sequencergui/viewmodel/drag_and_drop_helper.h>

#include <QMimeData>
#include <memory>

namespace
{

std::unique_ptr<QStandardItem> CreateItem(const std::string& name)
{
  auto result = std::make_unique<QStandardItem>(QString::fromStdString(name));
  result->setEditable(false);
  return result;
}

}  // namespace

namespace sequencergui
{

InstructionToolKitViewModel::InstructionToolKitViewModel(QObject* parent)
    : QStandardItemModel(parent)
{
}

QMimeData* InstructionToolKitViewModel::mimeData(const QModelIndexList& index_list) const
{
  if (!index_list.empty())
  {
    auto item = itemFromIndex(index_list.at(0));
    auto mime_data = CreateNewInstructionMimeData(item->data(Qt::DisplayRole).toString());
    return mime_data.release();
  }

  return nullptr;
}

void InstructionToolKitViewModel::PopulateModel(const std::vector<std::string> &object_types)
{
  auto parent_item = invisibleRootItem();

  for (const auto& name : object_types)
  {
    parent_item->appendRow(CreateItem(name).release());
  }
}


}  // namespace sequencergui
