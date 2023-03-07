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

#include "sequencergui/transform/variable_item_transform_utils.h"

#include <sequencergui/model/variable_item.h>

#include <mvvm/interfaces/sessionmodel_interface.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/sequencer/variable.h>

namespace sequencergui
{

// std::string GetValuesToJSONString(const variable_t *value)
//{
//   sup::dto::AnyValue anyvalue;
//   value->GetValue(anyvalue);
//   return sup::gui::GetValuesToJSONString(&anyvalue);
// }

void SetAnyValue(const anyvalue_t &anyvalue, VariableItem &variable_item)
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
  if (anyvalue_item->IsScalar())
  {
    anyvalue_item->SetDisplayName("value");
    anyvalue_item->SetToolTip(anyvalue_item->GetAnyTypeName());
  }

  if (model)
  {
    model->InsertItem(std::move(anyvalue_item), &variable_item, {});
  }
  else
  {
    variable_item.InsertItem(std::move(anyvalue_item), {});
  }
}

void SetAnyValueFromJsonType(const std::string &json_type, VariableItem &variable_item)
{
  auto anytype = ::sup::gui::AnyTypeFromJSONString(json_type);
  const sup::dto::AnyValue anyvalue(anytype);
  SetAnyValue(anyvalue, variable_item);
}

void UpdateAnyValue(const anyvalue_t &anyvalue, VariableItem &variable_item)
{
  if (auto existing_anyvalue_item = variable_item.GetAnyValueItem(); existing_anyvalue_item)
  {
    // If AnyValueItem already exists, we assume that its layout coincide with AnyValue.

    // updating existing AnyValueItem using temporary AnyValueItem
    auto temp_anyvalue_item = sup::gui::CreateItem(anyvalue);
    sup::gui::UpdateAnyValueItemData(*temp_anyvalue_item.get(), *existing_anyvalue_item);
  }
  else
  {
    // If AnyValueItem doesn't exist, we create new AnyValueItem using AnyValue provided.
    SetAnyValue(anyvalue, variable_item);
  }
}

}  // namespace sequencergui
