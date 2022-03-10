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

#include "anyvalueeditor/anyvalueutils.h"

#include "AnyType.h"

#include <algorithm>
#include <map>

namespace
{

const std::string kStructTypeName = "struct";
const std::string kScalarTypeName = "scalar";
const std::string kArrayTypeName = "array";

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

const std::map<sup::dto::TypeCode, std::string> kTypeMap = {
    {sup::dto::TypeCode::Empty, kEmptyTypeName},
    {sup::dto::TypeCode::Struct, kStructTypeName},
    {sup::dto::TypeCode::Array, kArrayTypeName},
    {sup::dto::TypeCode::Bool, kBooleanTypeName},
    {sup::dto::TypeCode::Char8, kChar8TypeName},
    {sup::dto::TypeCode::Int8, kInt8TypeName},
    {sup::dto::TypeCode::UInt8, kUInt8TypeName},
    {sup::dto::TypeCode::Int16, kInt16TypeName},
    {sup::dto::TypeCode::UInt16, kUInt16TypeName},
    {sup::dto::TypeCode::Int32, kInt32TypeName},
    {sup::dto::TypeCode::UInt32, kUint32TypeName},
    {sup::dto::TypeCode::Int64, kInt64TypeName},
    {sup::dto::TypeCode::UInt64, kUint64TypeName},
    {sup::dto::TypeCode::Float32, kFloat32TypeName},
    {sup::dto::TypeCode::Float64, kFloat64TypeName},
    {sup::dto::TypeCode::String, kStringTypeName}};

const std::vector<sup::dto::TypeCode> kScalars = {
    sup::dto::TypeCode::Bool,   sup::dto::TypeCode::Char8,   sup::dto::TypeCode::Int8,
    sup::dto::TypeCode::UInt8,  sup::dto::TypeCode::Int16,   sup::dto::TypeCode::UInt16,
    sup::dto::TypeCode::Int32,  sup::dto::TypeCode::UInt32,  sup::dto::TypeCode::Int64,
    sup::dto::TypeCode::UInt64, sup::dto::TypeCode::Float32, sup::dto::TypeCode::Float64,
    sup::dto::TypeCode::String};

}  // namespace

namespace anyvalueeditor
{

std::vector<std::string> GetScalarTypeNames()
{
  std::vector<std::string> result;
  std::transform(kScalars.begin(), kScalars.end(), std::back_inserter(result),
                 [](auto t)
                 {
                   auto it = kTypeMap.find(t);
                   return it == kTypeMap.end() ? std::string() : it->second;
                 });

  return result;
}

}  // namespace anyvalueeditor
