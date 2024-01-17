/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "attribute_item.h"

#include <sequencergui/model/item_constants.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <mvvm/model/item_utils.h>

namespace
{
inline const int kAnyTypeNameRole = 10;  // role to store type name
}  // namespace

namespace sequencergui
{

AttributeItem::AttributeItem() : CompoundItem(Type)
{
  SetFlag(mvvm::Appearance::kProperty, true);
}

std::unique_ptr<mvvm::SessionItem> AttributeItem::Clone(bool make_unique_id) const
{
  return std::make_unique<AttributeItem>(*this, make_unique_id);
}

void AttributeItem::SetAnyTypeName(const std::string &type_name)
{
  SetData(type_name, kAnyTypeNameRole);
  SetData(sup::gui::GetVariantFromScalarTypeName(type_name));
}

std::string AttributeItem::GetAnyTypeName() const
{
  return HasData(kAnyTypeNameRole) ? Data<std::string>(kAnyTypeNameRole) : std::string();
}

void AttributeItem::SetAttributeAsString(const std::string &value)
{
  SetData(mvvm::constants::kStringTypeName, kAnyTypeNameRole);
  mvvm::utils::ReplaceData(*this, mvvm::variant_t(value), mvvm::DataRole::kData);
}

bool AttributeItem::IsUnset() const
{
  auto data = Data();
  return std::holds_alternative<std::string>(data)
         && std::get<std::string>(data) == itemconstants::kUnsetValue;
}

void AttributeItem::MarkAsUnset()
{
  SetEditable(false);
  SetEnabled(false);
  mvvm::utils::ReplaceData(*this, itemconstants::kUnsetValue, mvvm::DataRole::kData);
}

}  // namespace sequencergui
