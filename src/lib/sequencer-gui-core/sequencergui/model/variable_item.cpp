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

#include "variable_item.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/standard_variable_items.h>

#include <sup/gui/model/anyvalue_item.h>
#include <sup/sequencer/variable.h>

namespace
{
const std::vector<std::string> kExpectedAnyValueItemTypes{
    sup::gui::AnyValueEmptyItem::Type, sup::gui::AnyValueScalarItem::Type,
    sup::gui::AnyValueArrayItem::Type, sup::gui::AnyValueStructItem::Type};

static inline const std::string kJsonType = "kJsonType";
static inline const std::string kJsonValue = "kJsonValue";

}  // namespace

namespace sequencergui
{
// ----------------------------------------------------------------------------
// VariableItem
// ----------------------------------------------------------------------------

static inline const std::string kName = "kName";
static inline const std::string kAnyValueTag = "kAnyValueTag";

VariableItem::VariableItem(const std::string &item_type) : CompoundItem(item_type)
{
  AddProperty(kName, std::string())->SetDisplayName("name");
}

std::unique_ptr<variable_t> VariableItem::CreateDomainVariable() const
{
  auto result = ::sequencergui::CreateDomainVariable(GetDomainType());

  if (GetType() != UnknownVariableItem::Type)
  {
    result->AddAttribute(domainconstants::kNameAttribute, Property<std::string>(kName));
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

    if (variable->HasAttribute(domainconstants::kNameAttribute))
    {
      SetProperty(kName, variable->GetAttribute(domainconstants::kNameAttribute));
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

std::string VariableItem::GetJsonType() const
{
  return Property<std::string>(kJsonType);
}

void VariableItem::SetJsonType(const std::string &value)
{
  SetProperty(kJsonType, value);
}

std::string VariableItem::GetJsonValue() const
{
  return Property<std::string>(kJsonValue);
}

void VariableItem::SetJsonValue(const std::string &value)
{
  SetProperty(kJsonValue, value);
}

sup::gui::AnyValueItem *VariableItem::GetAnyValueItem() const
{
  return GetItem<sup::gui::AnyValueItem>(kAnyValueTag);
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

//! Register properties representing json type and value.

void VariableItem::RegisterJsonTypeAndValue()
{
  AddProperty(kJsonType, std::string())
      ->SetDisplayName("json type")
      ->SetVisible(false)
      ->SetEditable(false);
  AddProperty(kJsonValue, std::string())
      ->SetDisplayName("json value")
      ->SetVisible(false)
      ->SetEditable(false);
}

//! Provides tag registration for AnyValueItem insertion.
//! The method is protected to allow derived classes to call it at the proper moment and so
//! manipulate the order of children.

void VariableItem::RegisterAnyValueItemTag()
{
  RegisterTag(mvvm::TagInfo(kAnyValueTag, 0, 1, kExpectedAnyValueItemTypes), true);
}

}  // namespace sequencergui
