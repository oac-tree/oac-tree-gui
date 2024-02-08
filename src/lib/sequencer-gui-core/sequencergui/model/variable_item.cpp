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

#include "variable_item.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/item_constants.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>

#include <mvvm/model/item_utils.h>

#include <sup/sequencer/variable.h>

namespace sequencergui
{

VariableItem::VariableItem(const std::string &item_type) : CompoundItem(item_type) {}

std::unique_ptr<variable_t> VariableItem::CreateDomainVariable() const
{
  auto result = ::sequencergui::CreateDomainVariable(GetDomainType());
  SetupDomainImpl(result.get());
  return result;
}

void VariableItem::InitFromDomain(const variable_t *variable, const anytype_registry_t *registry)
{
  InitFromDomainImpl(variable, registry);
}

std::string VariableItem::GetName() const
{
  return Property<std::string>(itemconstants::kName);
}

void VariableItem::SetName(const std::string &value)
{
  SetProperty(itemconstants::kName, value);
}

sup::gui::AnyValueItem *VariableItem::GetAnyValueItem() const
{
  return mvvm::utils::HasTag(*this, itemconstants::kAnyValueTag)
             ? GetItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag)
             : nullptr;
}

bool VariableItem::IsAvailable() const
{
  return true;
}

void VariableItem::SetIsAvailable(bool value)
{
  // Nothing to do for the base class. ConnectableItem::SetIsAvailable overrides that to show
  // connectable status in UI.
  (void)value;
}

void VariableItem::RegisterAnyValueItemTag()
{
  RegisterTag(sup::gui::CreateAnyValueTag(itemconstants::kAnyValueTag), true);
}

}  // namespace sequencergui
