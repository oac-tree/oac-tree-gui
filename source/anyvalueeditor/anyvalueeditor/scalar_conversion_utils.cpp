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

#include "anyvalueeditor/scalar_conversion_utils.h"

#include <anyvalueeditor/anyvalue_item.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/basic_scalar_types.h>

#include <functional>
#include <map>
#include <stdexcept>

namespace anyvalueeditor
{

template <typename T>
void SetDataFromScalarT(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)
{
  T val = anyvalue.As<T>();
  item.SetData(val);
}

template <>
void SetDataFromScalarT<sup::dto::int64>(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)
{
  auto val = anyvalue.As<sup::dto::int64>();
  item.SetData(static_cast<int>(val));
}

template <>
void SetDataFromScalarT<sup::dto::uint32>(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)
{
  auto val = anyvalue.As<sup::dto::uint32>();
  item.SetData(static_cast<int>(val));
}

template <>
void SetDataFromScalarT<sup::dto::uint64>(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)
{
  auto val = anyvalue.As<sup::dto::uint64>();
  item.SetData(static_cast<int>(val));
}

void SetDataFromScalar(const anyvalue_t &value, AnyValueItem &item)
{
  using sup::dto::TypeCode;
  using function_t = std::function<void(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)>;
  static std::map<TypeCode, function_t> conversion_map{
      {TypeCode::Bool, SetDataFromScalarT<sup::dto::boolean>},
      {TypeCode::Char8, SetDataFromScalarT<sup::dto::char8>},
      {TypeCode::Int8, SetDataFromScalarT<sup::dto::int8>},
      {TypeCode::UInt8, SetDataFromScalarT<sup::dto::uint8>},
      {TypeCode::Int16, SetDataFromScalarT<sup::dto::int16>},
      {TypeCode::UInt16, SetDataFromScalarT<sup::dto::uint16>},
      {TypeCode::Int32, SetDataFromScalarT<sup::dto::int32>},
      {TypeCode::UInt32, SetDataFromScalarT<sup::dto::uint32>},
      {TypeCode::Int64, SetDataFromScalarT<sup::dto::int64>},
      {TypeCode::UInt64, SetDataFromScalarT<sup::dto::uint64>},
      {TypeCode::Float32, SetDataFromScalarT<sup::dto::float32>},
      {TypeCode::Float64, SetDataFromScalarT<sup::dto::float64>},
      {TypeCode::String, SetDataFromScalarT<std::string>}};

  auto iter = conversion_map.find(value.GetTypeCode());
  if (iter == conversion_map.end())
  {
    throw std::runtime_error("Not a known scalar type code");
  }
  return iter->second(value, item);
}

sup::dto::AnyValue GetAnyValueFromScalar(const AnyValueItem &item)
{
//  using anyvalue_function_t =
//      std::function<void(const pvxs::Value& pvxs_value, sup::dto::AnyValue& anyvalue)>;

//  //! Correspondance of AnyValue type code to PVXS value function to assign scalars.
//  const std::map<sup::dto::TypeCode, anyvalue_function_t> kAssignToAnyValueScalarMap = {
//      {sup::dto::TypeCode::Bool, AssignToAnyValueScalar<sup::dto::boolean>},
//      {sup::dto::TypeCode::Char8, AssignToAnyValueScalar<sup::dto::uint8>},  // is it Ok?
//      {sup::dto::TypeCode::Int8, AssignToAnyValueScalar<sup::dto::int8>},
//      {sup::dto::TypeCode::UInt8, AssignToAnyValueScalar<sup::dto::uint8>},
//      {sup::dto::TypeCode::Int16, AssignToAnyValueScalar<sup::dto::int16>},
//      {sup::dto::TypeCode::UInt16, AssignToAnyValueScalar<sup::dto::uint16>},
//      {sup::dto::TypeCode::Int32, AssignToAnyValueScalar<sup::dto::int32>},
//      {sup::dto::TypeCode::UInt32, AssignToAnyValueScalar<sup::dto::uint32>},
//      {sup::dto::TypeCode::Int64, AssignToAnyValueScalar<sup::dto::int64>},
//      {sup::dto::TypeCode::UInt64, AssignToAnyValueScalar<sup::dto::uint64>},
//      {sup::dto::TypeCode::Float32, AssignToAnyValueScalar<sup::dto::float32>},
//      {sup::dto::TypeCode::Float64, AssignToAnyValueScalar<sup::dto::float64>},
//      {sup::dto::TypeCode::String, AssignToAnyValueScalar<std::string>}};

  return {};
}

}  // namespace anyvalueeditor
