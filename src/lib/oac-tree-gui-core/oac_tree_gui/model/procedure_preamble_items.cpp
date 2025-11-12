/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "procedure_preamble_items.h"

#include <mvvm/model/combo_property.h>
#include <mvvm/standarditems/container_item.h>

namespace oac_tree_gui
{

namespace
{
constexpr auto kTypeRegistrationMode = "kTypeRegistrationMode";
constexpr auto kTypeRegistrationString = "kTypeRegistrationString";
constexpr auto kTypeRegistrationContainerTag = "kTypeRegistrationContainerTag";
constexpr auto kPluginContainerTag = "kPluginContainerTag";
}  // namespace

// ----------------------------------------------------------------------------
// TypeRegistrationInfoItem
// ----------------------------------------------------------------------------

TypeRegistrationItem::TypeRegistrationItem()
    : CompoundItem(mvvm::GetTypeName<TypeRegistrationItem>())
{
  (void)AddProperty(kTypeRegistrationMode, mvvm::ComboProperty({"JSON file", "JSON string"}))
      .SetDisplayName("Mode");
  (void)AddProperty(kTypeRegistrationString, std::string())
      .SetDisplayName("String")
      .SetToolTip(
          "JSON string or a filename for a file containing the JSON representation of the type");
}

std::unique_ptr<mvvm::SessionItem> TypeRegistrationItem::Clone() const
{
  return std::make_unique<TypeRegistrationItem>(*this);
}

int TypeRegistrationItem::GetRegistrationMode() const
{
  auto combo = Property<mvvm::ComboProperty>(kTypeRegistrationMode);
  return combo.GetCurrentIndex();
}

void TypeRegistrationItem::SetRegistrationMode(int value)
{
  auto combo = Property<mvvm::ComboProperty>(kTypeRegistrationMode);
  combo.SetCurrentIndex(value);
  (void)SetProperty(kTypeRegistrationMode, combo);
}

std::string TypeRegistrationItem::GetRegistrationString() const
{
  return Property<std::string>(kTypeRegistrationString);
}

void TypeRegistrationItem::SetRegistrationString(const std::string& value)
{
  (void)SetProperty(kTypeRegistrationString, value);
}

// ----------------------------------------------------------------------------
// ProcedurePreambleItem
// ----------------------------------------------------------------------------

ProcedurePreambleItem::ProcedurePreambleItem()
    : CompoundItem(mvvm::GetTypeName<ProcedurePreambleItem>())
{
  (void)AddBranch<mvvm::ContainerItem>(kTypeRegistrationContainerTag)
      .SetDisplayName("Type registration");
  (void)AddBranch<mvvm::ContainerItem>(kPluginContainerTag).SetDisplayName("Plugins");
}

std::unique_ptr<mvvm::SessionItem> ProcedurePreambleItem::Clone() const
{
  return std::make_unique<ProcedurePreambleItem>(*this);
}

mvvm::ContainerItem* ProcedurePreambleItem::GetTypeRegistrationContainer() const
{
  return GetItem<mvvm::ContainerItem>(kTypeRegistrationContainerTag);
}

mvvm::ContainerItem* ProcedurePreambleItem::GetPluginContainer() const
{
  return GetItem<mvvm::ContainerItem>(kPluginContainerTag);
}

void ProcedurePreambleItem::AddTypeRegistration(int mode, const std::string& str)
{
  auto type_item = std::make_unique<TypeRegistrationItem>();
  auto type_item_ptr = type_item.get();
  (void)GetTypeRegistrationContainer()->InsertItem(std::move(type_item), mvvm::TagIndex::Append());

  type_item_ptr->SetRegistrationMode(mode);
  type_item_ptr->SetRegistrationString(str);
}

std::vector<std::pair<int, std::string> > ProcedurePreambleItem::GetTypeRegistrations() const
{
  std::vector<std::pair<int, std::string> > result;
  auto type_items = GetTypeRegistrationContainer()->GetItems<TypeRegistrationItem>(
      mvvm::TagIndex::GetDefaultTag());
  for (auto item : type_items)
  {
    result.push_back({item->GetRegistrationMode(), item->GetRegistrationString()});
  }

  return result;
}

std::vector<std::string> ProcedurePreambleItem::GetPluginPaths() const
{
  std::vector<std::string> result;
  auto plugin_name_property_items = GetPluginContainer()->GetItems(mvvm::TagIndex::GetDefaultTag());
  for (auto item : plugin_name_property_items)
  {
    result.push_back(item->Data<std::string>());
  }

  return result;
}

void ProcedurePreambleItem::AddPluginPath(const std::string& value)
{
  auto property_item = std::make_unique<mvvm::PropertyItem>();
  (void)property_item->SetData(value);
  (void)GetPluginContainer()->InsertItem(std::move(property_item), mvvm::TagIndex::Append());
}

void ProcedurePreambleItem::SetPluginPaths(const std::vector<std::string>& plugin_paths)
{
  // remove previous if plugin list is not the same
  if (plugin_paths == GetPluginPaths())
  {
    return;
  }

  GetPluginContainer()->Clear();

  for (const auto& value : plugin_paths)
  {
    AddPluginPath(value);
  }
}

}  // namespace oac_tree_gui
