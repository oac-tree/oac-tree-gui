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

#include "sequencergui/model/shallnotbenamedvalueutils.h"

#include "Variable.h"

#include <common/AnyValueHelper.h>

namespace sequi::DomainUtils
{

std::string GetJsonString(const anyvalue_t *value)
{
  ccs::types::char8 buffer[4096] = STRING_UNDEFINED;
  auto result = value->SerialiseInstance(buffer, 4096u);
  return std::string(buffer);
}

std::string GetValueJsonString(const variable_t *value)
{
  ::ccs::types::AnyValue anyvalue;
  value->GetValue(anyvalue);
  return GetJsonString(&anyvalue);
}

}  // namespace sequi::DomainUtils
