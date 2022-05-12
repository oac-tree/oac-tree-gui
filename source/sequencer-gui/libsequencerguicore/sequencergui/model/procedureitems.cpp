/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/procedureitems.h"

#include "Instruction.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/instructionitem.h"
#include "sequencergui/model/item_constants.h"
#include "sequencergui/model/standardinstructionitems.h"
#include "sequencergui/model/standardvariableitems.h"

#include "mvvm/model/itemutils.h"

namespace sequencergui
{
// ----------------------------------------------------------------------------
// InstructionContainerItem
// ----------------------------------------------------------------------------

InstructionContainerItem::InstructionContainerItem() : CompoundItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
              /*as_default*/ true);
}

std::vector<InstructionItem *> InstructionContainerItem::GetInstructions() const
{
  return GetItems<InstructionItem>(itemconstants::kChildInstructions);
}

// ----------------------------------------------------------------------------
// WorkspaceItem
// ----------------------------------------------------------------------------
static inline const std::string kVariableItems = "kVariableItems";

WorkspaceItem::WorkspaceItem() : CompoundItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kVariableItems), /*as_default*/ true);
}

std::vector<VariableItem *> WorkspaceItem::GetVariables() const
{
  return GetItems<VariableItem>(kVariableItems);
}

}  // namespace sequencergui
