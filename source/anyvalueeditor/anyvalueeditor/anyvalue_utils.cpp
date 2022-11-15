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

#include "anyvalueeditor/anyvalue_utils.h"

#include <sup/dto/anytype.h>
#include <sup/dto/anytype_registry.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/basic_scalar_types.h>
#include <sup/dto/json_value_parser.h>

#include <algorithm>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#include <stdexcept>

namespace
{

const std::map<sup::dto::TypeCode, std::string> kTypeCodeNameMap = {
    {sup::dto::TypeCode::Empty, sup::dto::kEmptyTypeName},
    {sup::dto::TypeCode::Struct, anyvalueeditor::kStructTypeName},
    {sup::dto::TypeCode::Array, anyvalueeditor::kArrayTypeName},
    {sup::dto::TypeCode::Bool, sup::dto::kBooleanTypeName},
    {sup::dto::TypeCode::Char8, sup::dto::kChar8TypeName},
    {sup::dto::TypeCode::Int8, sup::dto::kInt8TypeName},
    {sup::dto::TypeCode::UInt8, sup::dto::kUInt8TypeName},
    {sup::dto::TypeCode::Int16, sup::dto::kInt16TypeName},
    {sup::dto::TypeCode::UInt16, sup::dto::kUInt16TypeName},
    {sup::dto::TypeCode::Int32, sup::dto::kInt32TypeName},
    {sup::dto::TypeCode::UInt32, sup::dto::kUInt32TypeName},
    {sup::dto::TypeCode::Int64, sup::dto::kInt64TypeName},
    {sup::dto::TypeCode::UInt64, sup::dto::kUInt64TypeName},
    {sup::dto::TypeCode::Float32, sup::dto::kFloat32TypeName},
    {sup::dto::TypeCode::Float64, sup::dto::kFloat64TypeName},
    {sup::dto::TypeCode::String, sup::dto::kStringTypeName}};

const std::vector<sup::dto::TypeCode> kScalars = {
    sup::dto::TypeCode::Bool,   sup::dto::TypeCode::Char8,   sup::dto::TypeCode::Int8,
    sup::dto::TypeCode::UInt8,  sup::dto::TypeCode::Int16,   sup::dto::TypeCode::UInt16,
    sup::dto::TypeCode::Int32,  sup::dto::TypeCode::UInt32,  sup::dto::TypeCode::Int64,
    sup::dto::TypeCode::UInt64, sup::dto::TypeCode::Float32, sup::dto::TypeCode::Float64,
    sup::dto::TypeCode::String};

std::string GetName(sup::dto::TypeCode code)
{
  auto iter = kTypeCodeNameMap.find(code);
  return iter == kTypeCodeNameMap.end() ? std::string() : iter->second;
}

template <typename T>
variant_t ScalarToItemT()
{
  T val{};
  return variant_t(val);  // construct variant from given type
}

template <>
variant_t ScalarToItemT<sup::dto::int64>()
{
  return {0};  // returns variant based on `int` instead
}

template <>
variant_t ScalarToItemT<sup::dto::uint64>()
{
  return {0};  // returns variant based on `int` instead
}

template <>
variant_t ScalarToItemT<sup::dto::uint32>()
{
  return {0};  // returns variant based on `int` instead
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

sup::dto::TypeCode GetTypeCode(const std::string& name)
{
  auto iter = std::find_if(kTypeCodeNameMap.begin(), kTypeCodeNameMap.end(),
                           [name](auto item) { return item.second == name; });
  if (iter == kTypeCodeNameMap.end())
  {
    throw std::runtime_error("Error in TypeCode");
  }
  return iter->first;
}

variant_t GetVariantForAnyValueTypeName(const std::string& type_name)
{
  // The variant is intended for editing in cells of Qt trees and tables.
  // The concret type stored in it might be different from the AnyType as given by `type_name`.
  // For example, we are going to use `int` to edit `uint8` in GUI widgets. Necessary
  // limits will be provided by cell editors.

  using sup::dto::TypeCode;
  using function_t = std::function<variant_t()>;
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
  auto iter = conversion_map.find(GetTypeCode(type_name));
  if (iter == conversion_map.end())
  {
    throw std::runtime_error("Not a known scalar type code");
  }
  return iter->second();
}

bool IsScalarTypeName(const std::string& name)
{
  static const std::vector<std::string> expected_names = GetScalarTypeNames();
  return std::find(expected_names.begin(), expected_names.end(), name) != expected_names.end();
}

bool IsStructTypeName(const std::string& name)
{
  return name == kStructTypeName;
}

sup::dto::AnyValue AnyValueFromJSONFile(const std::string& filename)
{
  sup::dto::JSONAnyValueParser parser;
  parser.ParseFile(filename);
  return parser.MoveAnyValue();
}

}  // namespace anyvalueeditor
