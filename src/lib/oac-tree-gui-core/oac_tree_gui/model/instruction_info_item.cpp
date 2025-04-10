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

#include "instruction_info_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_automation_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/utils/string_utils.h>

#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/instruction_info_utils.h>

namespace oac_tree_gui
{

InstructionInfoItem::InstructionInfoItem() : InstructionItem(GetStaticType()) {}

std::string InstructionInfoItem::GetStaticType()
{
  return "InstructionInfoItem";
}

std::unique_ptr<mvvm::SessionItem> InstructionInfoItem::Clone() const
{
  return std::make_unique<InstructionInfoItem>(*this);
}

void InstructionInfoItem::InitFromDomainInfo(const sup::oac_tree::InstructionInfo &info)
{
  if (GetDomainType().empty())
  {
    SetupFromDomain(info);
  }

  SetDomainType(info.GetType());
  SetDisplayName(info.GetType());

  for (auto &[attr_name, attr_value] : info.GetAttributes())
  {
    if (attr_name == domainconstants::kNameAttribute)
    {
      SetName(attr_value);
    }

    else if (attr_name == domainconstants::kShowCollapsedAttribute)
    {
      // show_collapsed came as string, need to convert it to bool
      SetProperty(domainconstants::kShowCollapsedAttribute, mvvm::utils::StringToBool(attr_value));
    }

    else
    {
      // creating string properties from attributes solely for tooltips
      if (!mvvm::utils::HasTag(*this, attr_name))
      {
        AddProperty(attr_name, attr_value).SetEditable(false);
      }
      else
      {
        SetProperty(attr_name, attr_value);
      }
    }
  }
}

void InstructionInfoItem::InitFromDomainImpl(const instruction_t *instruction)
{
  (void)instruction;

  throw RuntimeException("This instruction is not intended to represent domain instructions");
}

void InstructionInfoItem::SetupDomainImpl(instruction_t *instruction) const
{
  (void)instruction;

  throw RuntimeException("This instruction is not intended to create domain instructions");
}

void InstructionInfoItem::SetupFromDomain(const sup::oac_tree::InstructionInfo &info)
{
  AddProperty(itemconstants::kName, std::string());
  RegisterChildrenTag(info.GetCategory(), *this);
  RegisterCommonProperties();
  RegisterShowCollapsedProperty(info.GetCategory(), info.GetType(), *this);
}

}  // namespace oac_tree_gui
