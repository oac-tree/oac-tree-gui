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

#include "variable_item.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/variable.h>

namespace oac_tree_gui
{

VariableItem::VariableItem(const std::string &item_type) : CompoundItem(item_type)
{
  SetFlag(mvvm::Appearance::kEditableDisplayName, true);
}

std::unique_ptr<variable_t> VariableItem::CreateDomainVariable() const
{
  auto result = ::oac_tree_gui::CreateDomainVariable(GetDomainType());
  SetupDomainImpl(result.get());
  return result;
}

void VariableItem::InitFromDomain(const variable_t *variable, const anytype_registry_t *registry)
{
  InitFromDomainImpl(variable, registry);
}

std::string VariableItem::GetName() const
{
  return GetDisplayName();
}

void VariableItem::SetName(const std::string &value)
{
  SetDisplayName(value);
}

sup::gui::AnyValueItem *VariableItem::GetAnyValueItem() const
{
  return mvvm::utils::HasTag(*this, itemconstants::kAnyValueTag)
             ? GetItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag)
             : nullptr;
}

bool VariableItem::IsAvailable() const
{
  return true;
}

void VariableItem::SetIsAvailable(bool value)
{
  // Nothing to do for the base class. ConnectableItem::SetIsAvailable overrides that to show
  // connectable status in UI.
  (void)value;
}

void VariableItem::RegisterAnyValueItemTag()
{
  RegisterTag(sup::gui::CreateAnyValueTag(itemconstants::kAnyValueTag), true);
}

}  // namespace oac_tree_gui
