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

AnyValueItem::AnyValueItem(const std::string& item_type) : CompoundItem(item_type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*as_default*/ true);
}

void AnyValueItem::SetAnyTypeName(const std::string& type_name)
{
  // saving type_name under own role
  SetData(type_name, kAnyTypeNameRole);

  if (IsScalar())
  {
    // for scalars we set variant initialised to the underlying type
    SetData(GetVariantForAnyValueTypeName(type_name));
  }
}

std::string AnyValueItem::GetAnyTypeName() const
{
  return HasData(kAnyTypeNameRole) ? Data<std::string>(kAnyTypeNameRole) : std::string();
}

bool AnyValueItem::IsScalar() const
{
  return IsScalarTypeName(GetAnyTypeName());
}

bool AnyValueItem::IsStruct() const
{
  return IsStructTypeName(GetAnyTypeName());
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
  SetData(GetVariantForAnyValueTypeName(type_name));
}

bool AnyValueScalarItem::IsScalar() const
{
  return true;
}

bool AnyValueScalarItem::IsStruct() const
{
  return false;
}

// ----------------------------------------------------------------------------
// AnyValueStructItem
// ----------------------------------------------------------------------------

AnyValueStructItem::AnyValueStructItem() : AnyValueItem(Type)
{
  //  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*as_default*/ true);
}

bool AnyValueStructItem::IsScalar() const
{
  return false;
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
//  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*as_default*/ true);
}

bool AnyValueArrayItem::IsScalar() const
{
  return false;
}

bool AnyValueArrayItem::IsStruct() const
{
  return false;
}

}  // namespace anyvalueeditor
