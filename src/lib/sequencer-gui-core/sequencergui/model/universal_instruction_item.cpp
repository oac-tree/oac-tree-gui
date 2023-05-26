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

#include <sup/sequencer/instruction.h>

namespace
{
// These attributes shouldn't be used from the domain to build properties.
const std::vector<std::string> kSkipDomainAttributeList({});

// these are properties that shouldn't go to domain
const std::vector<std::string> kSkipItemTagList({sequencergui::itemconstants::kChildInstructions});

}  // namespace

namespace sequencergui
{

UniversalInstructionItem::UniversalInstructionItem(const std::string &domain_type)
    : InstructionItem(domain_type.empty() ? Type : domain_type)
{
  if (!domain_type.empty())
  {
    // temporary domain variable is used to create default properties
    auto domain_variable = ::sequencergui::CreateDomainInstruction(domain_type);
    SetupFromDomain(domain_variable.get());
  }
}

std::unique_ptr<mvvm::SessionItem> UniversalInstructionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<UniversalInstructionItem>(*this, make_unique_id);
}

std::string UniversalInstructionItem::GetDomainType() const
{
  return m_domain_type;
}

//! Initialise instruction from domain item.
//! This is temporarily implementation which is used for all instructions, yet unknown for the  GUI.

void UniversalInstructionItem::InitFromDomainImpl(const instruction_t *instruction)
{
  if (m_domain_type.empty())
  {
    SetupFromDomain(instruction);
  }

  for (const auto &[attribute_name, item] : GetAttributeItems())
  {
    SetPropertyFromDomainAttribute(*instruction, attribute_name, *item);
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

  // for the moment any registered property has it's correspondance as domain attribute
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

void UniversalInstructionItem::SetupFromDomain(const instruction_t *variable)
{
  if (!m_domain_type.empty())
  {
    throw LogicErrorException("It is not possible to setup instruction twice");
  }

  m_domain_type = variable->GetType();

  SetDisplayName(variable->GetType());

  for (const auto &definition : variable->GetAttributeDefinitions())
  {
    if (!mvvm::utils::Contains(kSkipDomainAttributeList, definition.GetName()))
    {
      AddPropertyFromDefinition(definition, *this);
    }
  }

  // FIXME make it dependent on instruction base (decorator,  vs compound)
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
              /*as_default*/ true);
}

}  // namespace sequencergui
