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

#include "copy_and_paste_helper.h"

#include <oac_tree_gui/components/mime_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <sup/gui/components/item_filter_helper.h>
#include <sup/gui/components/mime_conversion_helper.h>

#include <mvvm/utils/container_utils.h>

#include <QMimeData>

namespace oac_tree_gui
{

std::unique_ptr<QMimeData> CreateInstructionCopyMimeData(const InstructionItem& instruction)
{
  const auto children = instruction.GetInstructions();
  // skip all children instructions during serialization
  auto filter_func = [&children](const mvvm::SessionItem& item)
  { return !mvvm::utils::Contains(children, &item); };

  auto result = std::make_unique<QMimeData>();
  sup::gui::CopyItemsToMimeData({&instruction}, kInstructionCopyMimeType, *result, filter_func);
  sup::gui::SetItemTypesToMime({&instruction}, kTopSelectedItemTypesMimeType, *result);
  return result;
}

std::unique_ptr<QMimeData> CreateInstructionTreeCopyMimeData(
    const std::vector<const InstructionItem*>& selection)
{
  auto result = std::make_unique<QMimeData>();

  auto top_level_selection =
      sup::gui::GetTopLevelSelection(mvvm::utils::CastItems<const mvvm::SessionItem>(selection));

  sup::gui::CopyItemsToMimeData(top_level_selection, kInstructionCopyMimeType, *result);
  sup::gui::SetItemTypesToMime(top_level_selection, kTopSelectedItemTypesMimeType, *result);
  return result;
}

std::unique_ptr<QMimeData> CreateInstructionSelectionCopyMimeData(
    const std::vector<const InstructionItem*>& selection)
{
  // accept all items that are instructions and in the selection list, accept all properties
  auto filter_func = [&selection](const mvvm::SessionItem& item)
  {
    if (item.GetTagIndex().GetTag() == itemconstants::kChildInstructions)
    {
      return mvvm::utils::Contains(selection, &item);
    }
    return true;  // all properties
  };

  auto top_level_selection =
      sup::gui::GetTopLevelSelection(mvvm::utils::CastItems<const mvvm::SessionItem>(selection));

  auto result = std::make_unique<QMimeData>();
  sup::gui::CopyItemsToMimeData(top_level_selection, kInstructionCopyMimeType, *result,
                                filter_func);
  sup::gui::SetItemTypesToMime(top_level_selection, kTopSelectedItemTypesMimeType, *result);

  return result;
}

std::vector<std::unique_ptr<mvvm::SessionItem> > CreateInstructions(const QMimeData* mime_data)
{
  if (mime_data == nullptr)
  {
    return {};
  }

  return sup::gui::CreateSessionItemsFromMimeData(*mime_data, kInstructionCopyMimeType);
}

}  // namespace oac_tree_gui
