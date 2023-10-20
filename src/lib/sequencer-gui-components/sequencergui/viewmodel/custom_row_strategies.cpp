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

#include "custom_row_strategies.h"

#include "custom_presentation_items.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/variable_item.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <map>

namespace
{

//! Creates view item representing is_available status.

std::unique_ptr<mvvm::ViewItem> CreateAvailableViewItem(mvvm::SessionItem *item)
{
  auto presentation = std::make_unique<sequencergui::ChannelPresentationItem>(item);
  return std::make_unique<mvvm::ViewItem>(std::move(presentation));
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
 * @brief Returns row of the table representing variable.
 */

std::vector<std::unique_ptr<mvvm::ViewItem>> CreateVariableRow(sequencergui::VariableItem &item)
{
  const std::string kEmptyValuePlaceholder("---");

  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  // column #0: it's name
  result.emplace_back(mvvm::CreateLabelViewItem(&item, item.GetName()));

  // column #1: scalar value, or placeholder
  if (auto anyvalue_item = item.GetAnyValueItem(); anyvalue_item)
  {
    if (anyvalue_item->IsScalar())
    {
      result.emplace_back(mvvm::CreateDataViewItem(anyvalue_item));
    }
    else
    {
      result.emplace_back(mvvm::CreateLabelViewItem(&item, kEmptyValuePlaceholder));
    }
  }

  // column #2: type of the variable (CA, Local, PVA-C, PVA-S
  result.emplace_back(mvvm::CreateLabelViewItem(&item, GetTypeString(item)));

  // column #3: Channel
  if (auto channel_property = sequencergui::GetChannelItem(item); channel_property)
  {
    result.emplace_back(mvvm::CreateDataViewItem(channel_property));
  }
  else
  {
    result.emplace_back(mvvm::CreateLabelViewItem(&item));
  }

  // column 4: is_available property
  if (auto is_available_property = sequencergui::GetIsAvailableItem(item); is_available_property)
  {
    result.emplace_back(CreateAvailableViewItem(is_available_property));
  }
  else
  {
    result.emplace_back(mvvm::CreateLabelViewItem(&item));
  }

  return result;
}

}  // namespace

namespace sequencergui
{

//! ---------------------------------------------------------------------------
//! VariableRowStrategy
//! ---------------------------------------------------------------------------

QStringList VariableRowStrategy::GetHorizontalHeaderLabels() const
{
  static QStringList result = {"Type", "Name"};
  return result;
}

std::vector<std::unique_ptr<mvvm::ViewItem>> VariableRowStrategy::ConstructRow(
    mvvm::SessionItem *item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  if (!item)
  {
    return result;
  }

  // If it's Variable itself, generate [display name, editable name]
  // If it's variabl's property, generate standart [property display name, property value name]

  result.emplace_back(mvvm::CreateDisplayNameViewItem(item));

  if (auto variable = dynamic_cast<VariableItem *>(item); variable)
  {
    result.emplace_back(mvvm::CreateDataViewItem(GetNameItem(*variable)));
  }
  else
  {
    result.emplace_back(mvvm::CreateDataViewItem(item));
  }

  return result;
}

//! ---------------------------------------------------------------------------
//! VariableTableRowStrategy
//! ---------------------------------------------------------------------------

// For local variable with scalar on board we build row of 5 elements:
// | var0       | 42      | Local   |             |     |

// For local variable with struct on board we build row of 5 elements:
// | var0       |         | Local   |             |     |
// |   struct   |         |         |             |     |
// |      value | 42      |         |             |     |

// For channel access variable with scalar on board
// | var0       | 42      | CA      | PV_CHANNEL0 | [x] |

QStringList VariableTableRowStrategy::GetHorizontalHeaderLabels() const
{
  static QStringList result = {"Name", "Value", "Type", "Channel", " "};
  return result;
}

std::vector<std::unique_ptr<mvvm::ViewItem>> VariableTableRowStrategy::ConstructRow(
    mvvm::SessionItem *item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  if (!item)
  {
    return result;
  }

  if (auto variable = dynamic_cast<VariableItem *>(item); variable)
  {
    return CreateVariableRow(*variable);
  }
  else
  {
    // It's AnyValueStructItem or AnyValueArrayItem
    result.emplace_back(mvvm::CreateDisplayNameViewItem(item));
    result.emplace_back(mvvm::CreateDataViewItem(item));
    // and empty placeholders for the rest
    result.emplace_back(mvvm::CreateLabelViewItem(item));
    result.emplace_back(mvvm::CreateLabelViewItem(item));
    result.emplace_back(mvvm::CreateLabelViewItem(item));
  }

  return result;
}

}  // namespace sequencergui
