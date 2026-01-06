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

#include "variable_info_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/variable_info.h>

namespace oac_tree_gui
{

VariableInfoItem::VariableInfoItem() : VariableItem(mvvm::GetTypeName<VariableInfoItem>()) {}

void VariableInfoItem::InitFromDomainInfo(const sup::oac_tree::VariableInfo& info)
{
  if (GetDomainType().empty())
  {
    SetupFromDomain(info);
  }

  SetDomainType(info.GetType());

  for (auto& [attr_name, attr_value] : info.GetAttributes())
  {
    if (attr_name == domainconstants::kNameAttribute)
    {
      SetName(attr_value);
    }

    else if (attr_name == domainconstants::kChannelAttribute)
    {
      // creating string properties from attributes solely for tooltips
      if (!mvvm::utils::HasTag(*this, attr_name))
      {
        (void)AddProperty(attr_name, attr_value).SetEditable(false);
      }
      else
      {
        (void)SetProperty(attr_name, attr_value);
      }
    }
  }

  if (itemconstants::kProvideVariableInfoInitialValue)
  {
    SetAnyValueFromDomainVariableInfo(info, *this);
  }
}

std::unique_ptr<mvvm::SessionItem> VariableInfoItem::Clone() const
{
  return std::make_unique<VariableInfoItem>(*this);
}

void VariableInfoItem::InitFromDomainImpl(const variable_t* variable,
                                          const anytype_registry_t* registry)
{
  (void)variable;
  (void)registry;

  throw RuntimeException("This variable is not intended to represent domain variable");
}

void VariableInfoItem::SetupDomainImpl(variable_t* variable) const
{
  (void)variable;
  throw RuntimeException("This variable is not intended to represent domain variable");
}

void VariableInfoItem::SetupFromDomain(const sup::oac_tree::VariableInfo& info)
{
  (void)info;
  RegisterAnyValueItemTag();
}

}  // namespace oac_tree_gui
