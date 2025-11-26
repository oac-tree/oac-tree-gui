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

#include "custom_row_strategies.h"

#include "custom_viewitem_factory.h"

#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/providers/viewitem.h>
#include <mvvm/providers/viewitem_factory.h>
#include <mvvm/style/mvvm_style_helper.h>

#include <QColor>
#include <QIcon>
#include <map>

namespace oac_tree_gui
{

namespace
{

QVariant GetValueIcon()
{
  // Our ColorIconEngine doesn't work with icons embedded into tree view
  // Have to set icons manually depending on theme
  if (mvvm::style::IsDarkTheme())
  {
    return QVariant::fromValue(QIcon(":/oac-tree/icons/file-tree-outline-light.svg"));
  }
  return QVariant::fromValue(QIcon(":/oac-tree/icons/file-tree-outline-dark.svg"));
}

/**
 * @brief Creates view item representing AnyValue in a column.
 */
std::unique_ptr<mvvm::ViewItem> CreateAnyValueViewItem(VariableItem &item)
{
  const std::string kSeeBelowPlaceholder("---");
  auto anyvalue_item = item.GetAnyValueItem();

  // if AnyValueItem is absent create empty placeholder
  if (!anyvalue_item)
  {
    return mvvm::CreateLabelViewItem(&item);
  }

  // if it is a scalar, create ViewItem looking at scalar's value
  if (anyvalue_item->IsScalar())
  {
    return mvvm::CreateDataViewItem(anyvalue_item);
  }

  // if it is a struct or array, create "see below" placeholder
  return mvvm::CreateLabelViewItem(&item, kSeeBelowPlaceholder);
}

/**
 * @brief Returns string representing type in 3rd column of variable table.
 */
std::string GetTypeString(const std::string &item_type)
{
  static const std::map<std::string, std::string> kNameMap = {
      {mvvm::GetTypeName<PvAccessClientVariableItem>(), std::string("PVA-C")},
      {mvvm::GetTypeName<PvAccessServerVariableItem>(), std::string("PVA-S")},
      {mvvm::GetTypeName<ChannelAccessVariableItem>(), std::string("CA")},
  };

  auto iter = kNameMap.find(item_type);
  return iter == kNameMap.end() ? item_type : iter->second;
}

/**
 * @brief Returns string representing type in 3rd column of variable tree.
 */
std::string GetTypeStringForVariableTree(const mvvm::SessionItem &item)
{
  if (auto anyvalue_item = dynamic_cast<const sup::gui::AnyValueItem *>(&item); anyvalue_item)
  {
    return anyvalue_item->GetAnyTypeName();
  }

  return item.GetType();
}

/**
 * @brief Returns row of the table representing variable.
 */

std::vector<std::unique_ptr<mvvm::ViewItem>> CreateVariableTableRow(VariableItem &item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  // column #0: it's a name
  (void)result.emplace_back(mvvm::CreateLabelViewItem(&item, item.GetName()));

  // column #1: scalar value, or placeholder
  (void)result.emplace_back(CreateAnyValueViewItem(item));

  // column #2: type of the variable (CA, Local, PVA-C, PVA-S
  (void)result.emplace_back(mvvm::CreateLabelViewItem(&item, GetTypeString(item.GetDomainType())));

  // column #3: Channel and is_available properties
  (void)result.emplace_back(CreateChannelPresentationViewItem(item));

  return result;
}

/**
 * @brief Returns row of the table representing variable.
 */
std::vector<std::unique_ptr<mvvm::ViewItem>> CreateVariableTreeRow(VariableItem &item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;
  (void)result.emplace_back(mvvm::CreateDisplayNameViewItem(&item));
  (void)result.emplace_back(mvvm::CreateLabelViewItem(&item));
  (void)result.emplace_back(mvvm::CreateLabelViewItem(&item, item.GetDomainType()));

  return result;
}

/**
 * @brief Returns row for the tree representing variable attribute.
 */
std::vector<std::unique_ptr<mvvm::ViewItem>> CreateVariableAttributeTreeRow(mvvm::SessionItem &item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  // If it's a tag corresponding to AnyValue, let's place an icon before display name.
  if (item.GetTagIndex().GetTag() == itemconstants::kAnyValueTag)
  {
    auto view_item = mvvm::CreateFixedDataViewItem(&item);
    (void)view_item->SetData(GetValueIcon(), Qt::DecorationRole);
    (void)view_item->SetData(QString::fromStdString(item.GetDisplayName()), Qt::DisplayRole);
    (void)result.emplace_back(std::move(view_item));
  }
  else
  {
    (void)result.emplace_back(mvvm::CreateDisplayNameViewItem(&item));
  }
  (void)result.emplace_back(mvvm::CreateDataViewItem(&item));
  (void)result.emplace_back(mvvm::CreateLabelViewItem(&item, GetTypeStringForVariableTree(item)));

  return result;
}

}  // namespace

//! ---------------------------------------------------------------------------
//! VariableRowStrategy
//! ---------------------------------------------------------------------------

std::size_t VariableRowStrategy::GetSize() const
{
  return 3U;
}

QStringList VariableRowStrategy::GetHorizontalHeaderLabels() const
{
  static const QStringList result = {"Name", "Value", "TypeName"};
  return result;
}

std::vector<std::unique_ptr<mvvm::ViewItem>> VariableRowStrategy::ConstructRowImpl(
    mvvm::SessionItem *item)
{
  if (auto variable = dynamic_cast<VariableItem *>(item); variable)
  {
    // If it's Variable itself, generate [editable name, empty label, modelType]
    return CreateVariableTreeRow(*variable);
  }

  // If it's variabl's property, generate standart [property display name, property value name]
  return CreateVariableAttributeTreeRow(*item);
}

//! ---------------------------------------------------------------------------
//! VariableTableRowStrategy
//! ---------------------------------------------------------------------------

// For local variable with scalar on board we build row of 4 elements:
// | var0       | 42      | Local   |                |

// For local variable with struct on board we build row of 4 elements:
// | var0       |         | Local   |                |
// |   struct   |         |         |                |
// |      value | 42      |         |                |

// For channel access variable with scalar on board
// | var0       | 42      | CA      | [x] PV_CHANNEL0 |

std::size_t VariableTableRowStrategy::GetSize() const
{
  return 4U;
}

QStringList VariableTableRowStrategy::GetHorizontalHeaderLabels() const
{
  static const QStringList result = {"Name", "Value", "Type", "Channel"};
  return result;
}

std::vector<std::unique_ptr<mvvm::ViewItem>> VariableTableRowStrategy::ConstructRowImpl(
    mvvm::SessionItem *item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  if (auto variable = dynamic_cast<VariableItem *>(item); variable)
  {
    return CreateVariableTableRow(*variable);
  }

  // It's AnyValueStructItem or AnyValueArrayItem and their branches
  (void)result.emplace_back(mvvm::CreateDisplayNameViewItem(item));
  (void)result.emplace_back(mvvm::CreateDataViewItem(item));
  // and empty placeholders for the rest
  (void)result.emplace_back(mvvm::CreateLabelViewItem(item));
  (void)result.emplace_back(mvvm::CreateLabelViewItem(item));

  return result;
}

//! ---------------------------------------------------------------------------
//! InstructionEditorRowStrategy
//! ---------------------------------------------------------------------------
std::size_t InstructionEditorRowStrategy::GetSize() const
{
  return 2U;
}

QStringList InstructionEditorRowStrategy::GetHorizontalHeaderLabels() const
{
  static const QStringList result = {"Type", "Name"};
  return result;
}

std::vector<std::unique_ptr<mvvm::ViewItem>> InstructionEditorRowStrategy::ConstructRowImpl(
    mvvm::SessionItem *item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  (void)result.emplace_back(mvvm::CreateDisplayNameViewItem(item));

  if (auto instruction = dynamic_cast<InstructionItem *>(item); instruction)
  {
    (void)result.emplace_back(mvvm::CreateDataViewItem(GetNameItem(*instruction)));
  }
  else
  {
    (void)result.emplace_back(mvvm::CreateLabelViewItem(item));
  }
  return result;
}

}  // namespace oac_tree_gui
