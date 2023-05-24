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

#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/transform_helpers.h>

#include <sup/sequencer/instruction.h>

namespace sequencergui
{

UniversalInstructionItem::UniversalInstructionItem() : InstructionItem(Type) {}

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
  m_domain_type = instruction->GetType();

  SetDisplayName(instruction->GetType());

  for (const auto &definition : instruction->GetAttributeDefinitions())
  {
    AddPropertyFromDefinition(definition, *this);
  }

  RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
              /*as_default*/ true);
}

void UniversalInstructionItem::SetupDomainImpl(instruction_t *instruction) const {}

}  // namespace sequencergui
