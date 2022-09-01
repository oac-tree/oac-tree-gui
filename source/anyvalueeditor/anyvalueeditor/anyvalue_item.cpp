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

#include "anyvalueeditor/anyvalue_item.h"

#include <anyvalueeditor/anyvalue_utils.h>

namespace anyvalueeditor
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
  SetAnyTypeName(kStructTypeName);
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*as_default*/ true);
}

bool AnyValueStructItem::IsStruct() const
{
  return true;
}

// ----------------------------------------------------------------------------
// AnyValueArrayItem
// ----------------------------------------------------------------------------

AnyValueArrayItem::AnyValueArrayItem() : AnyValueItem(Type)
{
  SetAnyTypeName(kArrayTypeName);
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*as_default*/ true);
}

bool AnyValueArrayItem::IsArray() const
{
  return true;
}

}  // namespace anyvalueeditor
