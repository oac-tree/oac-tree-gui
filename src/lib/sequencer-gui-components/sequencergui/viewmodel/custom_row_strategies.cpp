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

#include "custom_row_strategies.h"

#include "custom_viewitem_factory.h"

#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/style/style_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/style/style_helper.h>

#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <QColor>
#include <QIcon>
#include <map>

namespace
{

QVariant GetValueIcon()
{
  // Our ColorIconEngine doesn't work with icons embedded into tree view
  // Have to set icons manually depending on theme
  if (sup::gui::utils::IsDarkTheme())
  {
    return QVariant::fromValue(QIcon(":/oac-tree/icons/file-tree-outline-light.svg"));
  }
  return QVariant::fromValue(QIcon(":/oac-tree/icons/file-tree-outline-dark.svg"));
}

/**
 * @brief Creates view item representing AnyValue in a column.
 */
std::unique_ptr<mvvm::ViewItem> CreateAnyValueViewItem(sequencergui::VariableItem &item)
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
std::string GetTypeString(const mvvm::SessionItem &item)
{
  static const std::map<std::string, std::string> kNameMap = {
      {sequencergui::PvAccessClientVariableItem::Type, std::string("PVA-C")},
      {sequencergui::PvAccessServerVariableItem::Type, std::string("PVA-S")},
      {sequencergui::ChannelAccessVariableItem::Type, std::string("CA")},
  };

  auto iter = kNameMap.find(item.GetType());
  return iter == kNameMap.end() ? item.GetType() : iter->second;
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

std::vector<std::unique_ptr<mvvm::ViewItem>> CreateVariableTableRow(
    sequencergui::VariableItem &item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  // column #0: it's a name
  result.emplace_back(mvvm::CreateLabelViewItem(&item, item.GetName()));

  // column #1: scalar value, or placeholder
  result.emplace_back(CreateAnyValueViewItem(item));

  // column #2: type of the variable (CA, Local, PVA-C, PVA-S
  result.emplace_back(mvvm::CreateLabelViewItem(&item, GetTypeString(item)));

  // column #3: Channel and is_available properties
  result.emplace_back(CreateChannelPresentationViewItem(item));

  return result;
}

/**
 * @brief Returns row of the table representing variable.
 */
std::vector<std::unique_ptr<mvvm::ViewItem>> CreateVariableTreeRow(sequencergui::VariableItem &item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;
  result.emplace_back(mvvm::CreateDisplayNameViewItem(&item));
  result.emplace_back(mvvm::CreateLabelViewItem(&item));
  result.emplace_back(mvvm::CreateLabelViewItem(&item, item.GetType()));

  return result;
}

/**
 * @brief Returns row for the tree representing variable attribute.
 */
std::vector<std::unique_ptr<mvvm::ViewItem>> CreateVariableAttributeTreeRow(mvvm::SessionItem &item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  // If it's a tag corresponding to AnyValue, let's place an icon before display name.
  if (item.GetTagIndex().tag == sequencergui::itemconstants::kAnyValueTag)
  {
    auto view_item = mvvm::CreateFixedDataViewItem(&item);
    view_item->SetData(GetValueIcon(), Qt::DecorationRole);
    view_item->SetData(QString::fromStdString(item.GetDisplayName()), Qt::DisplayRole);
    result.emplace_back(std::move(view_item));
  }
  else
  {
    result.emplace_back(mvvm::CreateDisplayNameViewItem(&item));
  }
  result.emplace_back(mvvm::CreateDataViewItem(&item));
  result.emplace_back(mvvm::CreateLabelViewItem(&item, GetTypeStringForVariableTree(item)));

  return result;
}

}  // namespace

namespace sequencergui
{

//! ---------------------------------------------------------------------------
//! VariableRowStrategy
//! ---------------------------------------------------------------------------

int VariableRowStrategy::GetSize() const
{
  return 3;
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

int VariableTableRowStrategy::GetSize() const
{
  return 4;
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
  result.emplace_back(mvvm::CreateDisplayNameViewItem(item));
  result.emplace_back(mvvm::CreateDataViewItem(item));
  // and empty placeholders for the rest
  result.emplace_back(mvvm::CreateLabelViewItem(item));
  result.emplace_back(mvvm::CreateLabelViewItem(item));

  return result;
}

//! ---------------------------------------------------------------------------
//! InstructionEditorRowStrategy
//! ---------------------------------------------------------------------------
int InstructionEditorRowStrategy::GetSize() const
{
  return 2;
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

  result.emplace_back(mvvm::CreateDisplayNameViewItem(item));

  if (auto instruction = dynamic_cast<InstructionItem *>(item); instruction)
  {
    result.emplace_back(mvvm::CreateDataViewItem(GetNameItem(*instruction)));
  }
  else
  {
    result.emplace_back(mvvm::CreateLabelViewItem(item));
  }
  return result;
}

}  // namespace sequencergui
