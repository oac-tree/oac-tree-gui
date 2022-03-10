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
#include "BasicScalarTypes.h"

#include <algorithm>
#include <functional>
#include <map>
#include <stdexcept>

namespace
{

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

const std::map<sup::dto::TypeCode, std::string> kTypeCodeNameMap = {
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

std::string GetName(sup::dto::TypeCode code)
{
  auto it = kTypeCodeNameMap.find(code);
  return it == kTypeCodeNameMap.end() ? std::string() : it->second;
}

sup::dto::TypeCode GetTypeCode(const std::string& name)
{
  auto it = std::find_if(kTypeCodeNameMap.begin(), kTypeCodeNameMap.end(),
                         [name](auto item) { return item.second == name; });
  if (it == kTypeCodeNameMap.end())
  {
    throw std::runtime_error("Error in TypeCode");
  }
  return it->first;
}

template <typename T>
variant_t ScalarToItemT(sup::dto::TypeCode code)
{
  T val{};
  return variant_t(val);
}

template <>
variant_t ScalarToItemT<sup::dto::int64>(sup::dto::TypeCode code)
{
  return {0};
}

template <>
variant_t ScalarToItemT<sup::dto::uint64>(sup::dto::TypeCode code)
{
  return {0};
}

template <>
variant_t ScalarToItemT<sup::dto::uint32>(sup::dto::TypeCode code)
{
  return {0};
}

}  // namespace

namespace anyvalueeditor
{

std::vector<std::string> GetMainTypeNames()
{
  return {kStructTypeName, kArrayTypeName, kScalarTypeName};
}

std::vector<std::string> GetScalarTypeNames()
{
  std::vector<std::string> result;
  auto on_code = [](auto code) { return GetName(code); };
  std::transform(kScalars.begin(), kScalars.end(), std::back_inserter(result), on_code);
  return result;
}

variant_t GetVariantForAnyValueTypeName(const std::string& type_name)
{
  using sup::dto::TypeCode;
  using function_t = std::function<variant_t(sup::dto::TypeCode code)>;
  static std::map<TypeCode, function_t> conversion_map{
      {TypeCode::Bool, ScalarToItemT<sup::dto::boolean>},
      {TypeCode::Char8, ScalarToItemT<sup::dto::char8>},
      {TypeCode::Int8, ScalarToItemT<sup::dto::int8>},
      {TypeCode::UInt8, ScalarToItemT<sup::dto::uint8>},
      {TypeCode::Int16, ScalarToItemT<sup::dto::int16>},
      {TypeCode::UInt16, ScalarToItemT<sup::dto::uint16>},
      {TypeCode::Int32, ScalarToItemT<sup::dto::int32>},
      {TypeCode::UInt32, ScalarToItemT<sup::dto::uint32>},
      {TypeCode::Int64, ScalarToItemT<sup::dto::int64>},
      {TypeCode::UInt64, ScalarToItemT<sup::dto::uint64>},
      {TypeCode::Float32, ScalarToItemT<sup::dto::float32>},
      {TypeCode::Float64, ScalarToItemT<sup::dto::float64>},
      {TypeCode::String, ScalarToItemT<std::string>}};

  auto code = GetTypeCode(type_name);
  auto it = conversion_map.find(GetTypeCode(type_name));
  if (it == conversion_map.end())
  {
    throw std::runtime_error("Not a known scalar type code");
  }
  return it->second(code);
}

}  // namespace anyvalueeditor
