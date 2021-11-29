/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_SHALLNOTBENAMEDVALUEUTILS_H
#define SEQUENCERGUI_MODEL_SHALLNOTBENAMEDVALUEUTILS_H

//! @file shallnotbenamedvalueutils.h
//! Collection of utility functions to pretend that AnyValue doesn't exist in this world.

#include <string>

namespace ccs::types
{
class AnyValue;
}

using anyvalue_t = ccs::types::AnyValue;

namespace sequi::DomainUtils
{

std::string GetJsonString(const anyvalue_t* value);

}  // namespace sequi::DomainUtils

#endif  // SEQUENCERGUI_MODEL_SHALLNOTBENAMEDVALUEUTILS_H
