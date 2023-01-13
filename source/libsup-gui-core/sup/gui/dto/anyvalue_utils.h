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

#ifndef GUI_DTO_ANYVALUE_UTILS_H_
#define GUI_DTO_ANYVALUE_UTILS_H_

//! @file anyvalue_utils.h
//! Collection of utility functions to pretend that AnyValue doesn't exist in this world.

#include <sup/gui/dto/dto_types_fwd.h>

#include <string>

namespace sup::gui
{

std::string GetAnyValueToJSONString(const anyvalue_t* value, bool is_pretty = false);

std::string GetAnyTypeToJSONString(const anyvalue_t* value);

std::string GetValuesToJSONString(const anyvalue_t* value);

bool ParseStringToScalarAnyvalue(const std::string& str, anyvalue_t& value);

sup::dto::AnyValue AnyValueFromJSONFile(const std::string& filename);

}  // namespace sup::gui

#endif  // GUI_DTO_ANYVALUE_UTILS_H_
