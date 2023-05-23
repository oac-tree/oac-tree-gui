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

#include "universal_variable_item.h"

#include <sup/sequencer/variable.h>

namespace sequencergui
{

UniversalVariableItem::UniversalVariableItem() : VariableItem(Type)
{
  RegisterAnyValueItemTag();
}

std::unique_ptr<mvvm::SessionItem> UniversalVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<UniversalVariableItem>(*this, make_unique_id);
}

std::string UniversalVariableItem::GetDomainType() const
{
  return m_domain_name;
}

void UniversalVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  m_domain_name = variable->GetType();

  SetDisplayName(variable->GetType());
}

void UniversalVariableItem::SetupDomainImpl(variable_t *variable) const {}

}  // namespace sequencergui
