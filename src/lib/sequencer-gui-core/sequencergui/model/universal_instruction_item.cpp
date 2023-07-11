/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "universal_instruction_item.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/utils/container_utils.h>

#include <sup/sequencer/instruction.h>

#include <iostream>

namespace
{
const int kDomainTypeNameRole = 10;  // role to store type name

// These attributes shouldn't be used from the domain to build properties.
const std::vector<std::string> kSkipDomainAttributeList = {};

// These are properties that shouldn't go to domain. The list is empty for the moment because
// all GUI-only properties are hidden, and filtered out in
// UniversalInstructionItem::GetAttributeItems()
const std::vector<std::string> kSkipItemTagList = {};

}  // namespace

namespace sequencergui
{

UniversalInstructionItem::UniversalInstructionItem(const std::string &item_type)
    : InstructionItem(item_type)
{
  if (IsInstructionTypeAvailable(item_type))
  {
    SetDomainType(item_type);
  }
}

std::unique_ptr<mvvm::SessionItem> UniversalInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<UniversalInstructionItem>(*this, make_unique_id);
}

void UniversalInstructionItem::SetDomainType(const std::string &domain_type)
{
  // temporary domain instruction is used to create default properties
  auto domain_variable = ::sequencergui::CreateDomainInstruction(domain_type);
  SetupFromDomain(domain_variable.get());
}

std::string UniversalInstructionItem::GetDomainType() const
{
  return HasData(kDomainTypeNameRole) ? Data<std::string>(kDomainTypeNameRole) : std::string();
}

//! Initialise instruction from domain item.

void UniversalInstructionItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (GetDomainType().empty())
  {
    SetupFromDomain(instruction);
  }

  std::vector<std::string> processed_attribute_names;
  for (const auto &[attribute_name, item] : GetAttributeItems())
  {
    SetPropertyFromDomainAttribute(*instruction, attribute_name, *item);
    processed_attribute_names.push_back(attribute_name);
  }

  // creating property items representing custom attributes not present in attribute definitions
  for (const auto &[name, value] : instruction->GetStringAttributes())
  {
    if (!mvvm::utils::Contains(processed_attribute_names, name))
    {
      auto property = AddProperty<sup::gui::AnyValueScalarItem>(name);
      property->SetAnyTypeName(sup::dto::kStringTypeName);
      property->SetDisplayName(name);
      property->SetData(value);
    }
  }
}

void UniversalInstructionItem::SetupDomainImpl(instruction_t *instruction) const
{
  for (const auto &[attribute_name, item] : GetAttributeItems())
  {
    SetDomainAttribute(*item, attribute_name, *instruction);
  }
}

std::vector<UniversalInstructionItem::Attribute> UniversalInstructionItem::GetAttributeItems() const
{
  std::vector<UniversalInstructionItem::Attribute> result;

  // We need to collect all property items, which has correspondance  on the domain side and should
  // be used for the domain update. These items, currently, satisfy simple criteria:
  // - They are visible property items
  // - They are derived from AnyValueScalarItem

  auto properties = mvvm::utils::SinglePropertyItems(*this);

  for (const auto property : mvvm::utils::CastItems<sup::gui::AnyValueScalarItem>(properties))
  {
    auto [tag, index] = property->GetTagIndex();

    if (!mvvm::utils::Contains(kSkipItemTagList, tag))
    {
      // tag of property item should coincide to domain's attribute name
      result.push_back({tag, property});
    }
  }

  return result;
}

void UniversalInstructionItem::SetupFromDomain(const instruction_t *instruction)
{
  if (!GetDomainType().empty())
  {
    throw LogicErrorException("It is not possible to setup instruction twice");
  }

  SetData(instruction->GetType(), kDomainTypeNameRole);

  SetDisplayName(instruction->GetType());

  for (const auto &definition : instruction->GetAttributeDefinitions())
  {
    if (!mvvm::utils::Contains(kSkipDomainAttributeList, definition.GetName()))
    {
      AddPropertyFromDefinition(definition, *this);
    }
  }

  RegisterChildrenTag(*instruction, *this);

  RegisterCommonProperties();
}

}  // namespace sequencergui
