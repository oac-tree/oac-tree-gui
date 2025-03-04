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

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/instruction.h>

namespace oac_tree_gui
{

namespace
{

mvvm::ComboProperty CreateBehaviorProperty()
{
  mvvm::ComboProperty result({itemconstants::kNativeBehavior, itemconstants::kHiddenBehavior,
                              itemconstants::kSucceedBehavior, itemconstants::kFailBehavior});

  result.SetToolTips({"Instruction behaves according to the implementation",
                      "Instruction will be invisible and will not be executed",
                      "Instruction will always succeed", "Instruction will always fail"});

  return result;
}

}  // namespace

InstructionItem::InstructionItem(const std::string &item_type) : CompoundItem(item_type) {}

std::string InstructionItem::GetDomainType() const
{
  return HasData(itemconstants::kDomainTypeNameRole)
             ? Data<std::string>(itemconstants::kDomainTypeNameRole)
             : std::string();
}

void InstructionItem::SetDomainType(const std::string &domain_type)
{
  SetData(domain_type, itemconstants::kDomainTypeNameRole);
}

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
  auto result = ::oac_tree_gui::CreateDomainInstruction(GetDomainType());
  SetupDomainImpl(result.get());
  return result;
}

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
  AddProperty(itemconstants::kBehaviorTag, CreateBehaviorProperty())
      .SetToolTip("Modifier that allows to change the behavior");
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

}  // namespace oac_tree_gui
