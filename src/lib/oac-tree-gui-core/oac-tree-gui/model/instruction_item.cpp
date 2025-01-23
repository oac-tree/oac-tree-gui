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

#include "instruction_item.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/domain/domain_constants.h>
#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/model/item_constants.h>
#include <oac-tree-gui/model/universal_item_helper.h>
#include <oac-tree-gui/transform/attribute_item_transform_helper.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/instruction.h>

namespace sequencergui
{
// ----------------------------------------------------------------------------
// InstructionItem
// ----------------------------------------------------------------------------

InstructionItem::InstructionItem(const std::string &item_type) : CompoundItem(item_type) {}

std::unique_ptr<mvvm::SessionItem> InstructionItem::Clone() const
{
  throw NotImplementedException("InstructionItem::clone() should not be used");
}

void InstructionItem::InitFromDomain(const instruction_t *instruction)
{
  InitFromDomainImpl(instruction);
}

std::unique_ptr<instruction_t> InstructionItem::CreateDomainInstruction() const
{
  auto result = ::sequencergui::CreateDomainInstruction(GetDomainType());
  SetupDomainImpl(result.get());
  return result;
}

//! Returns all children instruction, if exist.

std::vector<InstructionItem *> InstructionItem::GetInstructions() const
{
  if (mvvm::utils::HasTag(*this, itemconstants::kChildInstructions))
  {
    return GetItems<InstructionItem>(itemconstants::kChildInstructions);
  }

  return {};
}

bool InstructionItem::IsRoot() const
{
  return Property<bool>(domainconstants::kIsRootAttribute);
}

void InstructionItem::SetIsRootFlag(bool value)
{
  SetAttribute(*this, domainconstants::kIsRootAttribute, value);
}

std::string InstructionItem::GetName() const
{
  return Property<std::string>(itemconstants::kName);
}

void InstructionItem::SetName(const std::string &name)
{
  SetProperty(itemconstants::kName, name);
}

std::string InstructionItem::GetStatus() const
{
  return Property<std::string>(itemconstants::kStatus);
}

void InstructionItem::SetStatus(const std::string &status)
{
  SetProperty(itemconstants::kStatus, status);
}

double InstructionItem::GetX() const
{
  return Property<double>(itemconstants::kXpos);
}

void InstructionItem::SetX(double value)
{
  SetProperty(itemconstants::kXpos, value);
}

double InstructionItem::GetY() const
{
  return Property<double>(itemconstants::kYpos);
}

void InstructionItem::SetY(double value)
{
  SetProperty(itemconstants::kYpos, value);
}

void InstructionItem::RegisterCommonProperties()
{
  AddProperty(itemconstants::kStatus, std::string())
      .SetDisplayName("Status")
      .SetVisible(false)
      .SetEditable(false);
  AddProperty(itemconstants::kXpos, 0.0).SetDisplayName("X").SetVisible(false);
  AddProperty(itemconstants::kYpos, 0.0).SetDisplayName("Y").SetVisible(false);
  AddProperty(itemconstants::kBreakpoint, 0)
      .SetDisplayName("breakpoint")
      .SetEditable(false)
      .SetVisible(false);

  AddShowCollapsedProperty(*this);
}

}  // namespace sequencergui
