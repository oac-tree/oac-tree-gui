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

#ifndef SEQUENCERGUI_TRANSFORM_VARIABLE_ITEM_TRANSFORM_UTILS_H_
#define SEQUENCERGUI_TRANSFORM_VARIABLE_ITEM_TRANSFORM_UTILS_H_

//! @file variable_item_transform_utils.h
//! Collection of utility functions to transform VariableItem to/from domain.

#include <memory>
#include <string>
#include <sequencergui/domain/sequencer_types.h>

namespace sequencergui
{

std::string GetValuesToJSONString(const variable_t* value);

//class VariableItem;
//class AnyValueItem;

////! Updates AnyValueItem on board
//void UpdateAnyValue(std::unique_ptr<AnyValueItem& anyvalue_item, VariableItem& variable_item);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_VARIABLE_ITEM_TRANSFORM_UTILS_H_

