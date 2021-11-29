/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
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
