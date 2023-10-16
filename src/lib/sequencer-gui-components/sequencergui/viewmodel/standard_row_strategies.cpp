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

#include "standard_row_strategies.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/variable_item.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

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

QStringList VariableTableRowStrategy::GetHorizontalHeaderLabels() const
{
  static QStringList result = {"Name", "Value", "Type"};
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
    result.emplace_back(mvvm::CreateLabelViewItem(variable, variable->GetName()));

    if (auto anyvalue_item = variable->GetAnyValueItem(); anyvalue_item)
    {
      if (anyvalue_item->IsScalar())
      {
        result.emplace_back(mvvm::CreateDataViewItem(anyvalue_item));
      }
      else
      {
        result.emplace_back(mvvm::CreateLabelViewItem(variable, std::string("---")));
      }
    }
    result.emplace_back(mvvm::CreateLabelViewItem(variable, variable->GetType()));
  }
  else
  {
    // It's AnyValueStructItem or AnyValueArrayItem
    result.emplace_back(mvvm::CreateDisplayNameViewItem(item));
    result.emplace_back(mvvm::CreateDataViewItem(item));
    result.emplace_back(mvvm::CreateLabelViewItem(item)); // placeholder for third column
  }

  return result;
}

}  // namespace sequencergui
