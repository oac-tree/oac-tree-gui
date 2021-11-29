/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/model/workspaceitems.h"

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

// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------

static inline const std::string kType = "kType";
static inline const std::string kValue = "kValue";

LocalVariableItem::LocalVariableItem() : VariableItem(Type)
{
  AddProperty(kType, std::string())->SetDisplayName("type");
  AddProperty(kValue, std::string())->SetDisplayName("value");
}

std::unique_ptr<variable_t> LocalVariableItem::CreateDomainVariable() const
{
  auto result = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
  result->AddAttribute(DomainConstants::kNameAttribute, GetName());
  result->AddAttribute(DomainConstants::kTypeAttribute, GetJsonType());
  result->AddAttribute(DomainConstants::kValueAttribute, GetJsonValue());
  return result;
}

void LocalVariableItem::InitFromDomain(const variable_t *variable)
{
  VariableItem::InitFromDomain(variable);
  if (variable->HasAttribute(DomainConstants::kNameAttribute))
  {
    SetProperty(kName, variable->GetAttribute(DomainConstants::kNameAttribute));
  }

  if (variable->HasAttribute(DomainConstants::kTypeAttribute))
  {
    SetProperty(kType, variable->GetAttribute(DomainConstants::kTypeAttribute));
  }

  if (variable->HasAttribute(DomainConstants::kValueAttribute))
  {
    SetProperty(kValue, variable->GetAttribute(DomainConstants::kValueAttribute));
  }
}

std::string LocalVariableItem::GetJsonType() const
{
  return Property<std::string>(kType);
}

void LocalVariableItem::SetJsonType(const std::string &value)
{
  SetProperty(kType, value);
}

std::string LocalVariableItem::GetJsonValue() const
{
  return Property<std::string>(kValue);
}

void LocalVariableItem::SetJsonValue(const std::string &value)
{
  SetProperty(kValue, value);
}

// ----------------------------------------------------------------------------
// UnknownVariableItem
// ----------------------------------------------------------------------------

UnknownVariableItem::UnknownVariableItem() : VariableItem(Type) {}

void UnknownVariableItem::InitFromDomain(const variable_t *variable)
{
  SetDisplayName(variable->GetType() + " (unknown)");
}

// ----------------------------------------------------------------------------
// WorkspaceItem
// ----------------------------------------------------------------------------
static inline const std::string kVariableItems = "kVariableItems";

WorkspaceItem::WorkspaceItem() : CompoundItem(Type)
{
  RegisterTag(ModelView::TagInfo::CreateUniversalTag(kVariableItems), /*as_default*/ true);
}

std::vector<VariableItem *> WorkspaceItem::GetVariables() const
{
  return GetItems<VariableItem>(kVariableItems);
}

}  // namespace sequi
