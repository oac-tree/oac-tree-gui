/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <mvvm/viewmodel/viewmodel_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMimeData>
#include <iostream>

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

}  // namespace sequencergui
