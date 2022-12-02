/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/dto/anyvalue_item.h"

#include <sup/gui/dto/conversion_utils.h>

namespace sup::gui
{

static inline const std::string kChildren = "kChildren";
static inline const int kAnyTypeNameRole = 10;  // role to store type name

// ----------------------------------------------------------------------------
// AnyValueItem
// ----------------------------------------------------------------------------

AnyValueItem::AnyValueItem(const std::string& item_type) : CompoundItem(item_type) {}

void AnyValueItem::SetAnyTypeName(const std::string& type_name)
{
  SetData(type_name, kAnyTypeNameRole);
}

std::string AnyValueItem::GetAnyTypeName() const
{
  return HasData(kAnyTypeNameRole) ? Data<std::string>(kAnyTypeNameRole) : std::string();
}

bool AnyValueItem::IsScalar() const
{
  return false;
}

bool AnyValueItem::IsStruct() const
{
  return false;
}

bool AnyValueItem::IsArray() const
{
  return false;
}

// ----------------------------------------------------------------------------
// AnyValueEmptyItem
// ----------------------------------------------------------------------------

AnyValueEmptyItem::AnyValueEmptyItem() : AnyValueItem(Type) {}

// ----------------------------------------------------------------------------
// AnyValueScalarItem
// ----------------------------------------------------------------------------

AnyValueScalarItem::AnyValueScalarItem() : AnyValueItem(Type) {}

void AnyValueScalarItem::SetAnyTypeName(const std::string& type_name)
{
  AnyValueItem::SetAnyTypeName(type_name);
  // setting default value for given type
  SetData(GetVariantForAnyValueTypeName(type_name));
}

bool AnyValueScalarItem::IsScalar() const
{
  return true;
}

// ----------------------------------------------------------------------------
// AnyValueStructItem
// ----------------------------------------------------------------------------

AnyValueStructItem::AnyValueStructItem() : AnyValueItem(Type)
{
  SetDisplayName(kStructTypeName);
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*as_default*/ true);
}

bool AnyValueStructItem::IsStruct() const
{
  return true;
}

AnyValueScalarItem* AnyValueStructItem::AddScalarField(const std::string& field_name,
                                                       const std::string& field_type,
                                                       const variant_t& value)
{
  auto child = InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  child->SetAnyTypeName(field_type);
  child->SetData(value);
  child->SetDisplayName(field_name);
  return child;
}

// ----------------------------------------------------------------------------
// AnyValueArrayItem
// ----------------------------------------------------------------------------

AnyValueArrayItem::AnyValueArrayItem() : AnyValueItem(Type)
{
  SetDisplayName(kArrayTypeName);
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*as_default*/ true);
}

bool AnyValueArrayItem::IsArray() const
{
  return true;
}

}  // namespace anyvalueeditor
