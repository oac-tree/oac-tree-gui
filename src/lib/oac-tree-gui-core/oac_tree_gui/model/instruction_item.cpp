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

#include "instruction_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/make_clone.h>

namespace oac_tree_gui
{

namespace
{

//! Returns the attribute item with the given name from the container, throws if it doesn't exist.
template <typename ContainerT>
auto& FindAttributeItem(ContainerT* attributes, const std::string& name,
                        const std::string& domain_type)
{
  auto* attribute =
      (attributes != nullptr) ? attributes->template FindItem<AttributeItem>(name) : nullptr;
  if (attribute == nullptr)
  {
    throw RuntimeException("Attribute [" + name + "] doesn't exist in instruction [" + domain_type
                           + "]");
  }
  return *attribute;
}

}  // namespace

InstructionItem::InstructionItem() : CompoundItem(mvvm::GetTypeName<InstructionItem>())
{
  (void)SetFlag(mvvm::Appearance::kEditableDisplayName, true);
}

std::string InstructionItem::GetDomainType() const
{
  return HasData(itemconstants::kDomainTypeNameRole)
             ? Data<std::string>(itemconstants::kDomainTypeNameRole)
             : std::string();
}

void InstructionItem::SetDomainType(const std::string& domain_type)
{
  (void)SetData(domain_type, itemconstants::kDomainTypeNameRole);
}

std::unique_ptr<mvvm::SessionItem> InstructionItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

std::vector<const InstructionItem*> InstructionItem::GetInstructions() const
{
  // FindItems is total, it returns an empty vector when the tag is not registered
  return FindItems<InstructionItem>(itemconstants::kChildInstructions);
}

std::vector<InstructionItem*> InstructionItem::GetInstructions()
{
  return FindItems<InstructionItem>(itemconstants::kChildInstructions);
}

const mvvm::CompoundItem* InstructionItem::GetAttributes() const
{
  return FindItem<mvvm::CompoundItem>(itemconstants::kAttributes);
}

mvvm::CompoundItem* InstructionItem::GetAttributes()
{
  return FindItem<mvvm::CompoundItem>(itemconstants::kAttributes);
}

bool InstructionItem::IsRoot() const
{
  return GetAttribute<bool>(domainconstants::kIsRootAttribute);
}

void InstructionItem::SetAttribute(const std::string& name, const mvvm::variant_t& value)
{
  auto& attribute = GetAttributeItem(name);
  if (value.index() != attribute.GetDefaultValue().index())
  {
    throw RuntimeException("Attribute [" + name + "] has type ["
                           + mvvm::utils::TypeName(attribute.GetDefaultValue()) + "], value type ["
                           + mvvm::utils::TypeName(value) + "]");
  }
  (void)attribute.SetValue(value);
}

void InstructionItem::SetAttributeText(const std::string& name, const std::string& text)
{
  auto& attribute = GetAttributeItem(name);
  if (!attribute.IsTextAllowed())
  {
    throw RuntimeException("Attribute [" + name + "] takes no text");
  }
  (void)attribute.SetText(text);
}

std::string InstructionItem::GetAttributeText(const std::string& name) const
{
  return GetAttributeItem(name).GetText();
}

void InstructionItem::UnsetAttribute(const std::string& name)
{
  auto& attribute = GetAttributeItem(name);
  if (!attribute.HasFlag(mvvm::Appearance::kNullable))
  {
    throw RuntimeException("Attribute [" + name + "] can't be unset");
  }
  (void)attribute.Unset();
}

const AttributeItem& InstructionItem::GetAttributeItem(const std::string& name) const
{
  return FindAttributeItem(GetAttributes(), name, GetDomainType());
}

AttributeItem& InstructionItem::GetAttributeItem(const std::string& name)
{
  return FindAttributeItem(GetAttributes(), name, GetDomainType());
}

mvvm::variant_t InstructionItem::GetAttributeValue(const std::string& name) const
{
  const auto& attribute = GetAttributeItem(name);
  if (attribute.GetState() != AttributeState::kValue)
  {
    throw RuntimeException("Attribute [" + name + "] has no value");
  }
  return attribute.Data();
}

void InstructionItem::SetIsRootFlag(bool value)
{
  SetAttribute(domainconstants::kIsRootAttribute, value);
}

std::string InstructionItem::GetName() const
{
  return GetDisplayName();
}

void InstructionItem::SetName(const std::string& name)
{
  (void)SetDisplayName(name);
}

InstructionStatus InstructionItem::GetStatus() const
{
  return GetInstructionStatus(Property<std::string>(itemconstants::kStatus));
}

void InstructionItem::SetStatus(InstructionStatus status)
{
  // internally we store status as a string so all views can show it
  (void)SetProperty(itemconstants::kStatus, ToString(status));
}

double InstructionItem::GetX() const
{
  return Property<double>(itemconstants::kXpos);
}

void InstructionItem::SetX(double value)
{
  (void)SetProperty(itemconstants::kXpos, value);
}

double InstructionItem::GetY() const
{
  return Property<double>(itemconstants::kYpos);
}

void InstructionItem::SetY(double value)
{
  (void)SetProperty(itemconstants::kYpos, value);
}

void InstructionItem::SetBehavior(const std::string& behavior_name)
{
  auto property = Property<mvvm::ComboProperty>(itemconstants::kBehaviorTag);
  property.SetValue(behavior_name);
  (void)SetProperty(itemconstants::kBehaviorTag, property);
}

std::string InstructionItem::GetBehavior() const
{
  if (mvvm::utils::HasTag(*this, itemconstants::kBehaviorTag))
  {
    return Property<mvvm::ComboProperty>(itemconstants::kBehaviorTag).GetValue();
  }

  return {};
}

}  // namespace oac_tree_gui
