/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/variableitem.h"

#include "Variable.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/domainutils.h"

namespace sequi
{
// ----------------------------------------------------------------------------
// VariableItem
// ----------------------------------------------------------------------------

static inline const std::string kName = "kName";

VariableItem::VariableItem(const std::string &item_type) : CompoundItem(item_type)
{
  AddProperty(kName, std::string())->SetDisplayName("name");
}

std::unique_ptr<variable_t> VariableItem::CreateDomainVariable() const
{
  throw std::runtime_error("Error in VariableItem: method is not implemented");
}

void VariableItem::InitFromDomain(const variable_t *variable)
{
  if (variable->HasAttribute(DomainConstants::kNameAttribute))
  {
    SetProperty(kName, variable->GetAttribute(DomainConstants::kNameAttribute));
  }
}

std::string VariableItem::GetName() const
{
  return Property<std::string>(kName);
}

void VariableItem::SetName(const std::string &value)
{
  SetProperty(kName, value);
}
}  // namespace sequi
