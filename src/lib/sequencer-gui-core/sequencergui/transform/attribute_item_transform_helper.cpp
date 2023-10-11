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

#include "attribute_item_transform_helper.h"

#include <sequencergui/model/attribute_item.h>

#include <sup/sequencer/attribute_definition.h>

namespace sequencergui
{

bool IsPlaceholderAttribute(const std::string &attribute_value)
{
  return attribute_value.find_first_of('$') == 0;
}

bool IsReferenceAttribute(const std::string &attribute_value)
{
  return attribute_value.find_first_of('@') == 0;
}

mvvm::SessionItem *AddPropertyFromDefinitionV2(const attribute_definition_t &attr,
                                             mvvm::CompoundItem &item)
{
  // Use attribute name for display name and tag name of the new property item.
  auto property = item.AddProperty<AttributeItem>(attr.GetName());
  property->SetAnyTypeName(attr.GetType().GetTypeName());  // will set default value too
  property->SetDisplayName(attr.GetName());
  return property;
}

}  // namespace sequencergui
