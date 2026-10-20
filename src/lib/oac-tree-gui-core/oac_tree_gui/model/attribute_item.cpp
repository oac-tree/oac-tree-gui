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

#include "attribute_item.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/make_clone.h>

namespace oac_tree_gui
{

namespace
{

//! Returns true if the given variant holds a string.
bool IsString(const mvvm::variant_t& value)
{
  return std::holds_alternative<std::string>(value);
}

//! Returns true if both variants hold the same type.
bool HasSameType(const mvvm::variant_t& lhs, const mvvm::variant_t& rhs)
{
  return lhs.index() == rhs.index();
}

}  // namespace

AttributeItem::AttributeItem() : mvvm::SessionItem(mvvm::GetTypeName<AttributeItem>())
{
  SetStrategy(this, &AttributeItem::OnSetData);
}

std::unique_ptr<mvvm::SessionItem> AttributeItem::Clone() const
{
  auto result = mvvm::MakeClone(*this);
  result->ActivateStrategies();
  return result;
}

void AttributeItem::SetAttributeType(const mvvm::variant_t& default_value, bool is_nullable)
{
  (void)SetData(default_value, mvvm::DataRole::kDefaultValue);
  (void)SetFlag(mvvm::Appearance::kNullable, is_nullable && !IsString(default_value));
  if (!HasFlag(mvvm::Appearance::kNullable) && !HasData(mvvm::DataRole::kData))
  {
    (void)SetData(default_value);
  }
}

mvvm::variant_t AttributeItem::GetDefaultValue() const
{
  return Data(mvvm::DataRole::kDefaultValue);
}

bool AttributeItem::HasText() const
{
  return IsString(Data()) && !IsString(GetDefaultValue());
}

void AttributeItem::ActivateStrategies()
{
  SetStrategy(this, &AttributeItem::OnSetData);
}

bool AttributeItem::OnSetData(mvvm::SessionItem* item, const mvvm::variant_t& value,
                              mvvm::role_t role)
{
  if (role != mvvm::DataRole::kData)
  {
    return mvvm::utils::SetData(*item, value, role);
  }

  const bool is_no_data = !mvvm::utils::IsValid(value);

  // Setting empty variant is allowed only for nullable data.
  // Settings any other variant is allowed in 1) if types coincide 2) New data is string ($par)
  const bool is_accepted = is_no_data ? HasFlag(mvvm::Appearance::kNullable)
                                      : (HasSameType(value, GetDefaultValue()) || IsString(value));
  if (!is_accepted)
  {
    return false;
  }

  if (is_no_data || !HasData(role) || HasSameType(value, Data(role)))
  {
    return mvvm::utils::SetData(*item, value, role);
  }

  // value and text have different types: remove the data, then set the new one
  mvvm::utils::BeginMacro(*this, "Change attribute");
  (void)mvvm::utils::SetData(*item, mvvm::variant_t{}, role);
  const bool result = mvvm::utils::SetData(*item, value, role);
  mvvm::utils::EndMacro(*this);
  return result;
}

}  // namespace oac_tree_gui
