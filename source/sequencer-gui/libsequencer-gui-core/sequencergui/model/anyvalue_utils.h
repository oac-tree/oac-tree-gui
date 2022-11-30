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

#ifndef SEQUENCERGUI_MODEL_SHALL_NOT_BE_NAMED_VALUE_UTILS_H_
#define SEQUENCERGUI_MODEL_SHALL_NOT_BE_NAMED_VALUE_UTILS_H_

//! @file shallnotbenamedvalueutils.h
//! Collection of utility functions to pretend that AnyValue doesn't exist in this world.

#include <sequencergui/model/sequencer_types.h>

#include <sequencergui/model/dto_types_fwd.h>
#include <string>

namespace sequencergui::DomainUtils
{

std::string GetAnyValueToJSONString(const anyvalue_t* value);

std::string GetAnyTypeToJSONString(const anyvalue_t* value);

std::string GetValuesToJSONString(const anyvalue_t* value);

std::string GetValuesToJSONString(const variable_t* value);

bool ParseStringToScalarAnyvalue(const std::string& str, anyvalue_t& value);

}  // namespace sequencergui::DomainUtils

#endif  // SEQUENCERGUI_MODEL_SHALL_NOT_BE_NAMED_VALUE_UTILS_H_
