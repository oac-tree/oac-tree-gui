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

#ifndef SEQUENCERGUI_TRANSFORM_VARIABLE_ITEM_TRANSFORM_UTILS_H_
#define SEQUENCERGUI_TRANSFORM_VARIABLE_ITEM_TRANSFORM_UTILS_H_

//! @file variable_item_transform_utils.h
//! Collection of utility functions to transform VariableItem to/from domain.

#include <sequencergui/domain/sequencer_types.h>

#include <sup/gui/dto/dto_types_fwd.h>

#include <memory>
#include <string>

namespace sup::gui
{
class AnyValueItem;
}

namespace sequencergui
{

class VariableItem;

std::string GetValuesToJSONString(const variable_t* value);

//! Updates AnyValueItem on board of variable item using given `anyvalue`.
void UpdateAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_VARIABLE_ITEM_TRANSFORM_UTILS_H_
