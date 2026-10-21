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

#include <oac_tree_gui/model/item_constants.h>

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

void AttributeItem::SetAttributeType(const mvvm::variant_t& default_value, bool is_nullable,
                                     bool allow_text)
{
  const bool is_string = IsString(default_value);
  (void)SetData(default_value, mvvm::DataRole::kDefaultValue);
  (void)SetFlag(mvvm::Appearance::kNullable, is_nullable && !is_string);
  (void)SetData(allow_text && !is_string, itemconstants::kAllowTextRole);
  if (!HasFlag(mvvm::Appearance::kNullable) && !HasData(mvvm::DataRole::kData))
  {
    (void)SetData(default_value);
  }
}

mvvm::variant_t AttributeItem::GetDefaultValue() const
{
  return Data(mvvm::DataRole::kDefaultValue);
}

bool AttributeItem::IsTextAllowed() const
{
  return HasData(itemconstants::kAllowTextRole) && Data<bool>(itemconstants::kAllowTextRole);
}

AttributeState AttributeItem::GetState() const
{
  if (HasData(itemconstants::kAttributeTextRole))
  {
    return AttributeState::kText;
  }
  return HasData() ? AttributeState::kValue : AttributeState::kNotSet;
}

std::string AttributeItem::GetText() const
{
  return HasData(itemconstants::kAttributeTextRole)
             ? Data<std::string>(itemconstants::kAttributeTextRole)
             : std::string{};
}

bool AttributeItem::SetText(const std::string& text)
{
  return SetData(text, itemconstants::kAttributeTextRole);
}

bool AttributeItem::SetValue(const mvvm::variant_t& value)
{
  if (!mvvm::utils::IsValid(value) || !HasSameType(value, GetDefaultValue()))
  {
    return false;
  }
  return ReplaceTextWithData(value);
}

bool AttributeItem::Unset()
{
  if (!HasFlag(mvvm::Appearance::kNullable))
  {
    return false;
  }
  return ReplaceTextWithData(mvvm::variant_t{});
}

void AttributeItem::ActivateStrategies()
{
  SetStrategy(this, &AttributeItem::OnSetData);
}

bool AttributeItem::OnSetData(mvvm::SessionItem* item, const mvvm::variant_t& value,
                              mvvm::role_t role)
{
  if (role == mvvm::DataRole::kData)
  {
    const bool is_accepted = mvvm::utils::IsValid(value) ? HasSameType(value, GetDefaultValue())
                                                         : HasFlag(mvvm::Appearance::kNullable);
    return is_accepted && mvvm::utils::SetData(*item, value, role);
  }

  if (role == itemconstants::kAttributeTextRole)
  {
    if (!IsTextAllowed() || (mvvm::utils::IsValid(value) && !IsString(value)))
    {
      return false;
    }
    const bool is_empty = !mvvm::utils::IsValid(value) || std::get<std::string>(value).empty();
    return mvvm::utils::SetData(*item, is_empty ? mvvm::variant_t{} : value, role);
  }

  return mvvm::utils::SetData(*item, value, role);
}

bool AttributeItem::ReplaceTextWithData(const mvvm::variant_t& value)
{
  if (GetState() != AttributeState::kText)
  {
    return SetData(value);
  }

  mvvm::utils::BeginMacro(*this, "Change attribute");
  (void)SetText({});
  (void)SetData(value);
  mvvm::utils::EndMacro(*this);
  return true;
}

}  // namespace oac_tree_gui
