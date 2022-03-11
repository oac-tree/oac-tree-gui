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

#ifndef ANYVALUEEDITOR_ANYVALUEUTILS_H
#define ANYVALUEEDITOR_ANYVALUEUTILS_H

#include "mvvm/core/variant.h"

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

const std::string kEmptyTypeName = "empty";
const std::string kBooleanTypeName = "bool";
const std::string kChar8TypeName = "char8";
const std::string kInt8TypeName = "int8";
const std::string kUInt8TypeName = "uint8";
const std::string kInt16TypeName = "int16";
const std::string kUInt16TypeName = "uint16";
const std::string kInt32TypeName = "int32";
const std::string kUint32TypeName = "uint32";
const std::string kInt64TypeName = "int64";
const std::string kUint64TypeName = "uint64";
const std::string kFloat32TypeName = "float32";
const std::string kFloat64TypeName = "float64";
const std::string kStringTypeName = "string";

std::vector<std::string> GetMainTypeNames();

std::vector<std::string> GetScalarTypeNames();

//! Returns variant carrying type corresponding to AnyValue type name. The value of the variant will
//! be default initialized. See implementation for more explanations.
variant_t GetVariantForAnyValueTypeName(const std::string& type_name);

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEUTILS_H
