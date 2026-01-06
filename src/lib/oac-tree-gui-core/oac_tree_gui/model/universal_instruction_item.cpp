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

#include "universal_instruction_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/instruction.h>

namespace
{

//! @brief Checks if given attribute shall be exposed to the GUI.
//! Some domain attributes shall not be used to build GUI item properties.
//!
//! @param domain_attribute_name The name of the attribute in domain attribute definition.
bool IsDomainAttributeToExpose(const std::string& domain_attribute_name)
{
  static const std::vector<std::string> kSkipDomainAttributeList = {
      oac_tree_gui::domainconstants::kTypeAttribute,  // handled via AnyValueItem
      oac_tree_gui::domainconstants::kValueAttribute  // handled via AnyValueItem
  };

  return !mvvm::utils::Contains(kSkipDomainAttributeList, domain_attribute_name);
}

/**
 * @brief Returns list of properties that shouldn't go to domain.
 */
const std::vector<std::string>& GetSkipItemTagList()
{
  static const std::vector<std::string> kSkipItemTagList = {
      oac_tree_gui::itemconstants::kAnyValueTag};
  return kSkipItemTagList;
}

}  // namespace

namespace oac_tree_gui
{

UniversalInstructionItem::UniversalInstructionItem()
    : InstructionItem(mvvm::GetTypeName<UniversalInstructionItem>())
{
}

UniversalInstructionItem::UniversalInstructionItem(const std::string& item_type)
    : InstructionItem(item_type)
{
  SetDomainTypeImpl(item_type);
}

std::unique_ptr<mvvm::SessionItem> UniversalInstructionItem::Clone() const
{
  return std::make_unique<UniversalInstructionItem>(*this);
}

void UniversalInstructionItem::SetDomainType(const std::string& domain_type)
{
  SetDomainTypeImpl(domain_type);
}

void UniversalInstructionItem::InitFromDomainImpl(const instruction_t* instruction)
{
  if (GetDomainType().empty())
  {
    SetupFromDomain(instruction);
  }

  std::vector<std::string> processed_attribute_names;
  for (const auto& [attribute_name, item] : GetAttributeItems())
  {
    SetPropertyFromDomainAttribute(*instruction, attribute_name, *item);
    processed_attribute_names.push_back(attribute_name);
  }

  // creating property items representing custom attributes not present in attribute definitions
  for (const auto& [name, value] : instruction->GetStringAttributes())
  {
    if (!mvvm::utils::Contains(processed_attribute_names, name) && IsDomainAttributeToExpose(name))
    {
      auto& property = AddProperty<sup::gui::AnyValueScalarItem>(name);
      property.SetAnyTypeName(sup::dto::kStringTypeName);
      (void)property.SetDisplayName(name);
      (void)property.SetData(value);
    }
  }

  if (mvvm::utils::HasTag(*this, oac_tree_gui::itemconstants::kAnyValueTag))
  {
    // FIXME What to do with registry of types (see UniversalVariableItem::InitFromDomainImpl
    SetAnyValueFromDomainInstruction(*instruction, *this);
  }
}

void UniversalInstructionItem::SetupDomainImpl(instruction_t* instruction) const
{
  for (const auto& [attribute_name, item] : GetAttributeItems())
  {
    SetDomainAttribute(*item, attribute_name, *instruction);
  }

  auto anyvalue_item = GetAnyValueItem(*this);
  if ((anyvalue_item != nullptr) && GetAttributeExposedFlag(*anyvalue_item))
  {
    SetJsonAttributesFromItem(*anyvalue_item, *instruction);
  }
}

void UniversalInstructionItem::SetDomainTypeImpl(const std::string& domain_type)
{
  // temporary domain instruction is used to create default properties
  auto domain_instruction = ::oac_tree_gui::CreateDomainInstruction(domain_type);
  SetupFromDomain(domain_instruction.get());
}

std::vector<UniversalInstructionItem::Attribute> UniversalInstructionItem::GetAttributeItems() const
{
  std::vector<UniversalInstructionItem::Attribute> result;

  // We need to collect all property items, which has correspondance  on the domain side and should
  // be used for the domain update. These items, currently, satisfy simple criteria:
  // - They are visible property items
  // - They are derived from AnyValueItem

  auto properties = mvvm::utils::SinglePropertyItems(*this);

  for (const auto property : mvvm::utils::CastItems<sup::gui::AnyValueItem>(properties))
  {
    auto tag = property->GetTagIndex().GetTag();

    if (!mvvm::utils::Contains(GetSkipItemTagList(), tag))
    {
      // tag of property item should coincide with the domain attribute name
      result.push_back({tag, property});
    }
  }

  return result;
}

void UniversalInstructionItem::SetupFromDomain(const instruction_t* instruction)
{
  if (!GetDomainType().empty())
  {
    throw LogicErrorException("It is not possible to setup instruction twice");
  }

  (void)SetData(instruction->GetType(), itemconstants::kDomainTypeNameRole);

  (void)SetDisplayName(instruction->GetType());

  for (const auto& definition : instruction->GetAttributeDefinitions())
  {
    if (IsDomainAttributeToExpose(definition.GetName()))
    {
      (void)AddPropertyFromDefinition(definition, *this);
    }
  }

  RegisterChildrenTag(*instruction, *this);

  RegisterCommonProperties();

  AddShowCollapsedProperty(*this);
}

}  // namespace oac_tree_gui
