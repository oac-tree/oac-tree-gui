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

#ifndef SEQUENCERGUI_DOMAIN_ANYVALUE_UTILS_H_
#define SEQUENCERGUI_DOMAIN_ANYVALUE_UTILS_H_

//! @file anyvalue_utils.h
//! Collection of utility functions to pretend that AnyValue doesn't exist in this world.

#include <sequencergui/domain/dto_types_fwd.h>
#include <sequencergui/model/sequencer_types.h>

#include <string>

namespace sequencergui::DomainUtils
{

std::string GetAnyValueToJSONString(const anyvalue_t* value, bool is_pretty = false);

std::string GetAnyTypeToJSONString(const anyvalue_t* value);

std::string GetValuesToJSONString(const anyvalue_t* value);

std::string GetValuesToJSONString(const variable_t* value);

bool ParseStringToScalarAnyvalue(const std::string& str, anyvalue_t& value);

sup::dto::AnyValue AnyValueFromJSONFile(const std::string& filename);

}  // namespace sequencergui::DomainUtils

#endif  // SEQUENCERGUI_DOMAIN_ANYVALUE_UTILS_H_
