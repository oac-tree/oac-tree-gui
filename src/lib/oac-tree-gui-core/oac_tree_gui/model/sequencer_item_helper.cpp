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

#include "sequencer_item_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/utils/container_utils.h>

#include <vector>

namespace
{

std::vector<std::string> GetDomainDecoratorNames()
{
  std::vector<std::string> result{oac_tree_gui::domainconstants::kForceSuccessInstructionType,
                                  oac_tree_gui::domainconstants::kIncludeInstructionType,
                                  oac_tree_gui::domainconstants::kListenInstructionType,
                                  oac_tree_gui::domainconstants::kRepeatInstructionType,
                                  oac_tree_gui::domainconstants::kInverterInstructionType};
  return result;
}

mvvm::SessionItem *GetPropertyItem(const mvvm::SessionItem &parent, const std::string &tag)
{
  return mvvm::utils::HasTag(parent, tag) ? parent.GetItem(tag) : nullptr;
}

}  // namespace

namespace oac_tree_gui
{

bool IsCompoundInstruction(const InstructionItem *instruction)
{
  return instruction->GetTaggedItems()->HasTag(itemconstants::kChildInstructions);
}

bool IsDecoratorInstruction(const InstructionItem *instruction)
{
  static const std::vector<std::string> domain_names = GetDomainDecoratorNames();
  return mvvm::utils::Contains(domain_names, instruction->GetDomainType());
}

mvvm::SessionItem *GetNameItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, itemconstants::kName);
}

mvvm::SessionItem *GetStatusItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, itemconstants::kStatus);
}

mvvm::SessionItem *GetBreakpointItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, itemconstants::kBreakpoint);
}

mvvm::SessionItem *GetIsAvailableItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, itemconstants::kIsAvailable);
}

mvvm::SessionItem *GetChannelItem(const mvvm::SessionItem &parent)
{
  return GetPropertyItem(parent, domainconstants::kChannelAttribute);
}

}  // namespace oac_tree_gui
