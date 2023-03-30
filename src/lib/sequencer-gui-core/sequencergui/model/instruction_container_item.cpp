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

#include "instruction_container_item.h"

#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/item_constants.h>

namespace sequencergui
{

InstructionContainerItem::InstructionContainerItem() : CompoundItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
              /*as_default*/ true);
}

std::unique_ptr<mvvm::SessionItem> InstructionContainerItem::Clone(bool make_unique_id) const
{
  return std::make_unique<InstructionContainerItem>(*this, make_unique_id);
}

std::vector<InstructionItem *> InstructionContainerItem::GetInstructions() const
{
  return GetItems<InstructionItem>(itemconstants::kChildInstructions);
}

}  // namespace sequencergui
