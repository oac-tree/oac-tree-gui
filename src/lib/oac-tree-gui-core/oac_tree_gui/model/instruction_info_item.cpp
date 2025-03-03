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

#include "instruction_info_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/item_constants.h>

namespace oac_tree_gui
{

InstructionInfoItem::InstructionInfoItem() : InstructionItem(GetStaticType())
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
              /*as_default*/ true);
  RegisterCommonProperties();
}

std::string InstructionInfoItem::GetStaticType()
{
  return "InstructionInfoItem";
}

void InstructionInfoItem::InitFromDomainImpl(const instruction_t *instruction)
{
  (void)instruction;

  throw RuntimeException("This instruction is not intended to represent domain instructions");
}

void InstructionInfoItem::SetupDomainImpl(instruction_t *instruction) const
{
  (void)instruction;

  throw RuntimeException("This instruction is not intended to create domain instructions");
}

std::unique_ptr<mvvm::SessionItem> InstructionInfoItem::Clone() const
{
  return std::make_unique<InstructionInfoItem>(*this);
}

}  // namespace oac_tree_gui
