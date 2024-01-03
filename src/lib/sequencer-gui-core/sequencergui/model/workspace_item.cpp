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

#include "workspace_item.h"

#include <sequencergui/model/variable_item.h>

namespace sequencergui
{
static inline const std::string kVariableItems = "kVariableItems";

WorkspaceItem::WorkspaceItem() : CompoundItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kVariableItems), /*as_default*/ true);
}

std::unique_ptr<mvvm::SessionItem> WorkspaceItem::Clone(bool make_unique_id) const
{
  return std::make_unique<WorkspaceItem>(*this, make_unique_id);
}

std::vector<VariableItem *> WorkspaceItem::GetVariables() const
{
  return GetItems<VariableItem>(kVariableItems);
}

int WorkspaceItem::GetVariableCount() const
{
  return static_cast<int>(GetVariables().size());
}

}  // namespace sequencergui
