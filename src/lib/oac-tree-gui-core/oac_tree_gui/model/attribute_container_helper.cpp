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

#include "attribute_container_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{

namespace
{

//! Returns the attribute item with the given name from the attribute container of the item, throws
//! if it doesn't exist.
template <typename ItemT>
auto& FindAttributeItem(ItemT& item, const std::string& name)
{
  auto* attributes = item.template FindItem<mvvm::CompoundItem>(itemconstants::kAttributes);
  auto* attribute =
      (attributes != nullptr) ? attributes->template FindItem<AttributeItem>(name) : nullptr;
  if (attribute == nullptr)
  {
    const auto domain_type =
        item.HasData(itemconstants::kDomainTypeNameRole)
            ? item.template Data<std::string>(itemconstants::kDomainTypeNameRole)
            : std::string();
    throw RuntimeException("Attribute [" + name + "] doesn't exist in " + item.GetType() + " ["
                           + domain_type + "]");
  }
  return *attribute;
}

}  // namespace

const AttributeItem& GetAttributeItem(const mvvm::SessionItem& item, const std::string& name)
{
  return FindAttributeItem(item, name);
}

AttributeItem& GetAttributeItem(mvvm::SessionItem& item, const std::string& name)
{
  return FindAttributeItem(item, name);
}

void SetAttributeValue(mvvm::SessionItem& item, const std::string& name,
                       const mvvm::variant_t& value)
{
  auto& attribute = GetAttributeItem(item, name);
  if (value.index() != attribute.GetDefaultValue().index())
  {
    throw RuntimeException("Attribute [" + name + "] has type ["
                           + mvvm::utils::TypeName(attribute.GetDefaultValue()) + "], value type ["
                           + mvvm::utils::TypeName(value) + "]");
  }
  (void)attribute.SetValue(value);
}

mvvm::variant_t GetAttributeValue(const mvvm::SessionItem& item, const std::string& name)
{
  const auto& attribute = GetAttributeItem(item, name);
  if (attribute.GetState() != AttributeState::kValue)
  {
    throw RuntimeException("Attribute [" + name + "] has no value");
  }
  return attribute.Data();
}

void UnsetAttributeValue(mvvm::SessionItem& item, const std::string& name)
{
  auto& attribute = GetAttributeItem(item, name);
  if (!attribute.HasFlag(mvvm::Appearance::kNullable))
  {
    throw RuntimeException("Attribute [" + name + "] can't be unset");
  }
  (void)attribute.Unset();
}

}  // namespace oac_tree_gui
