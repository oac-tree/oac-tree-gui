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

#include "anyvalueeditor/anyvalueitem.h"

#include "anyvalueeditor/anyvalueutils.h"

namespace
{
bool IsScalar(const std::string& name)
{
  return name != anyvalueeditor::kStructTypeName && name != anyvalueeditor::kArrayTypeName;
}
}  // namespace

namespace anyvalueeditor
{

static inline const std::string kChildren = "kChildren";
static inline const int kAnyTypeNameRole = 10;  // role to store type name

AnyValueItem::AnyValueItem() : CompoundItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*as_default*/ true);
}

void AnyValueItem::SetAnyTypeName(const std::string& type_name)
{
  if (IsScalar(type_name))
  {
    // for scalars we set variant initialised to the underlying type
    SetData(GetVariantForAnyValueTypeName(type_name));
  }

  // saving type_name under own role
  SetData(type_name, kAnyTypeNameRole);
}

std::string AnyValueItem::GetAnyTypeName() const
{
  return HasData(kAnyTypeNameRole) ? Data<std::string>(kAnyTypeNameRole) : std::string();
}

}  // namespace anyvalueeditor
