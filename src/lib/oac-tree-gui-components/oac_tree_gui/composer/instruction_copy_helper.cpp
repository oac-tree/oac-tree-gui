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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_copy_helper.h"

#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/viewmodel/drag_and_drop_helper.h>

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

  return sup::gui::CreateCopyMimeData(instruction, kCopyInstructionMimeType, filter_func);
}

std::vector<std::unique_ptr<mvvm::SessionItem> > CreateInstructions(const QMimeData* mime_data)
{
  return sup::gui::CreateSessionItems(mime_data, kCopyInstructionMimeType);
}

std::unique_ptr<QMimeData> CreateInstructionTreeCopyMimeData(const InstructionItem& instruction)
{
  return sup::gui::CreateCopyMimeData(instruction, kCopyInstructionMimeType, {});
}

}  // namespace oac_tree_gui
