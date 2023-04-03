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

#ifndef SEQUENCERGUI_TRANSFORM_VARIABLE_TRANSFORM_HELPER_H_
#define SEQUENCERGUI_TRANSFORM_VARIABLE_TRANSFORM_HELPER_H_

//! Collection of helper functions to transform VariableItem to/from domain.

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sup/gui/core/dto_types_fwd.h>

#include <memory>
#include <string>

namespace sup::gui
{
class AnyValueItem;
}

namespace sequencergui
{

class VariableItem;

// std::string GetValuesToJSONString(const variable_t* value);

//! Sets AnyValueItem on board of \it variable_item using given \it anyvalue.
//! If AnyValueItem already exist, it will be replaced.
void SetAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

//! Sets AnyValueItem on board of \it variable_item using \it json_type provided.
void SetAnyValueFromJsonType(const std::string& json_type, VariableItem& variable_item);

//! Sets AnyValueItem on board of \it variable_item using \it json_type and json_value provided.
void SetAnyValueFromJsonTypeAndValue(const std::string& json_type, const std::string& json_value,
                                     VariableItem& variable_item);

//! Updates existing AnyValueItem on board of \it variable_item using given \it anyvalue.
//! It is expected that the layout of existing AnyValueItem matches the layout of \it anyvalue.
void UpdateAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

//! Set attribute to given value. If the value is empty string, do nothing.
void AddNonEmptyAttribute(const std::string& attribute_name, const std::string& attribute_value,
                          variable_t& variable);

//! Sets JSON type attribute from given \it item to the domain variable.
void SetJsonTypeAttribute(const VariableItem& item, variable_t& variable);

//! Sets JSON value attribute from given \it item to the domain variable.
void SetJsonValueAttribute(const VariableItem& item, variable_t& variable);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_VARIABLE_TRANSFORM_HELPER_H_
