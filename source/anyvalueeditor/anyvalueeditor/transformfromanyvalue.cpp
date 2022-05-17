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

#include "anyvalueeditor/transformfromanyvalue.h"

#include "sup/dto/AnyValue.h"
#include "sup/dto/AnyValueHelper.h"
#include "sup/dto/BasicScalarTypes.h"
#include "anyvalueeditor/anyvalueitem.h"
#include "anyvalueeditor/anyvalueitembuilder.h"

#include <functional>
#include <map>
#include <stdexcept>

namespace anyvalueeditor
{

template <typename T>
void ScalarToItemT(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)
{
  T val = anyvalue.As<T>();
  item.SetData(val);
}

template <>
void ScalarToItemT<sup::dto::int64>(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)
{
  auto val = anyvalue.As<sup::dto::int64>();
  item.SetData(static_cast<int>(val));
}

template <>
void ScalarToItemT<sup::dto::uint32>(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)
{
  auto val = anyvalue.As<sup::dto::uint32>();
  item.SetData(static_cast<int>(val));
}

template <>
void ScalarToItemT<sup::dto::uint64>(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)
{
  auto val = anyvalue.As<sup::dto::uint64>();
  item.SetData(static_cast<int>(val));
}

void ScalarToItem(const anyvalue_t &value, AnyValueItem &item)
{
  using sup::dto::TypeCode;
  using function_t = std::function<void(const sup::dto::AnyValue &anyvalue, AnyValueItem &item)>;
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

  auto it = conversion_map.find(value.GetTypeCode());
  if (it == conversion_map.end())
  {
    throw std::runtime_error("Not a known scalar type code");
  }
  return it->second(value, item);
}

void PopulateItem(const sup::dto::AnyValue *domain, AnyValueItem *item)
{
  AnyValueItemBuilder m_builder(item);
  sup::dto::SerializeAnyValue(*domain, m_builder);
}

}  // namespace anyvalueeditor
