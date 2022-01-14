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

#include "sequencergui/model/variableitem.h"

#include "Variable.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/sequenceritems.h"

namespace sequencergui
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
  auto result = DomainUtils::CreateDomainVariable(GetDomainType());

  if (GetType() != UnknownVariableItem::Type)
  {
    result->AddAttribute(DomainConstants::kNameAttribute, Property<std::string>(kName));
  }

  SetupDomainImpl(result.get());
  return result;
}

void VariableItem::InitFromDomain(const variable_t *variable)
{
  if (GetType() != UnknownVariableItem::Type)
  {
    if (variable->GetType() != GetDomainType())
    {
      throw std::runtime_error("Error in VariableItem: domain instruction type '"
                               + variable->GetType() + "' doesn't match expected '"
                               + GetDomainType() + "' type.");
    }

    if (variable->HasAttribute(DomainConstants::kNameAttribute))
    {
      SetProperty(kName, variable->GetAttribute(DomainConstants::kNameAttribute));
    }
  }

  InitFromDomainImpl(variable);
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
