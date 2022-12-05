/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/transform/variable_item_transform_utils.h"

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <sequencergui/model/variable_item.h>
#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/anyvalue_item.h>
#include <sup/gui/dto/anyvalue_utils.h>
#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/variable.h>

namespace sequencergui
{

std::string GetValuesToJSONString(const variable_t *value)
{
  sup::dto::AnyValue anyvalue;
  value->GetValue(anyvalue);
  return sup::gui::GetValuesToJSONString(&anyvalue);
}

void UpdateAnyValue(const anyvalue_t &anyvalue, VariableItem &variable_item)
{
  // we will be acting through the model, if it exists, to allow signaling
  auto model = variable_item.GetModel();

  // in current implementation we remove old AnyValueItem, if it exists
  if (auto prev_item = variable_item.GetAnyValueItem(); prev_item)
  {
    if (model)
    {
      model->RemoveItem(prev_item);
    }
    else
    {
      variable_item.TakeItem(variable_item.TagIndexOfItem(prev_item));
    }
  }

  // Inserting new AnyValueItem
  auto anyvalue_item = sup::gui::CreateItem(anyvalue);
  if (model)
  {
    model->InsertItem(std::move(anyvalue_item), &variable_item, {});
  }
  else
  {
    variable_item.InsertItem(std::move(anyvalue_item), {});
  }
}

}  // namespace sequencergui
