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

std::unique_ptr<mvvm::SessionItem> InstructionContainerItem::Clone() const
{
  return std::make_unique<InstructionContainerItem>(*this);
}

std::vector<InstructionItem *> InstructionContainerItem::GetInstructions() const
{
  return GetItems<InstructionItem>(itemconstants::kChildInstructions);
}

int InstructionContainerItem::GetInstructionCount() const
{
  return static_cast<int>(GetInstructions().size());
}

}  // namespace sequencergui
