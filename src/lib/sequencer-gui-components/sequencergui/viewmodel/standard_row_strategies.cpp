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

#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace sequencergui
{

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

}  // namespace sequencergui
