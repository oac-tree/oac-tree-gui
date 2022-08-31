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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_UTILS_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_UTILS_H_

#include <anyvalueeditor/dto_types_fwd.h>
#include <mvvm/core/variant.h>

#include <string>
#include <vector>

namespace anyvalueeditor
{

struct FieldContext
{
  std::string name;     //! Name of the field
  std::string type;     //! Major type (scalar, array, struct)
  std::string subtype;  //! Scalar type
};

const std::string kStructTypeName = "struct";
const std::string kArrayTypeName = "array";
const std::string kScalarTypeName = "scalar";

std::vector<std::string> GetMainTypeNames();

std::vector<std::string> GetScalarTypeNames();

//! Returns any_value type code from the name.
sup::dto::TypeCode GetTypeCode(const std::string& name);

//! Returns variant carrying type corresponding to AnyValue type name. The value of the variant will
//! be default initialized. See implementation for more explanations.
variant_t GetVariantForAnyValueTypeName(const std::string& type_name);

//! Returns true if given name corresponds to a scalar.
bool IsScalarTypeName(const std::string& name);

//! Returns true if given name corresponds to a struct.
bool IsStructTypeName(const std::string& name);

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_UTILS_H_
