/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/model/shallnotbenamedvalueutils.h"

#include <common/AnyValueHelper.h>

namespace sequi::DomainUtils
{

std::string GetJsonString(const anyvalue_t *value)
{
  ccs::types::char8 buffer[4096] = STRING_UNDEFINED;
  auto result = value->SerialiseInstance(buffer, 4096u);
  return std::string(buffer);
}

}  // namespace sequi::DomainUtils
