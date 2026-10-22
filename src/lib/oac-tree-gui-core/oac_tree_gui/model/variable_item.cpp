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

#include "variable_item.h"

#include <oac_tree_gui/model/item_constants.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/make_clone.h>

namespace oac_tree_gui
{

VariableItem::VariableItem() : CompoundItem(mvvm::GetTypeName<VariableItem>())
{
  (void)SetFlag(mvvm::Appearance::kEditableDisplayName, true);
  (void)AddProperty(itemconstants::kIsAvailable, false)
      .SetDisplayName("connected")
      .SetEditable(false)
      .SetVisible(false);
}

std::unique_ptr<mvvm::SessionItem> VariableItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

std::string VariableItem::GetDomainType() const
{
  return HasData(itemconstants::kDomainTypeNameRole)
             ? Data<std::string>(itemconstants::kDomainTypeNameRole)
             : std::string();
}

void VariableItem::SetDomainType(const std::string& domain_type)
{
  (void)SetData(domain_type, itemconstants::kDomainTypeNameRole);
}

std::string VariableItem::GetName() const
{
  return GetDisplayName();
}

void VariableItem::SetName(const std::string& value)
{
  (void)SetDisplayName(value);
}

const mvvm::CompoundItem* VariableItem::GetAttributes() const
{
  return FindItem<mvvm::CompoundItem>(itemconstants::kAttributes);
}

mvvm::CompoundItem* VariableItem::GetAttributes()
{
  return FindItem<mvvm::CompoundItem>(itemconstants::kAttributes);
}

void VariableItem::SetAttribute(const std::string& name, const mvvm::variant_t& value)
{
  SetAttributeValue(*this, name, value);
}

void VariableItem::UnsetAttribute(const std::string& name)
{
  UnsetAttributeValue(*this, name);
}

const sup::gui::AnyValueItem* VariableItem::GetAnyValueItem() const
{
  return FindItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag);
}

sup::gui::AnyValueItem* VariableItem::GetAnyValueItem()
{
  return FindItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag);
}

bool VariableItem::IsAvailable() const
{
  return Property<bool>(itemconstants::kIsAvailable);
}

void VariableItem::SetIsAvailable(bool value)
{
  (void)SetProperty(itemconstants::kIsAvailable, value);
}

}  // namespace oac_tree_gui
