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

std::map<sup::dto::TypeCode, std::string> CreateTypeMap()
{
  std::map<sup::dto::TypeCode, std::string> result = {
      {sup::dto::TypeCode::Empty, "empty"},     {sup::dto::TypeCode::Struct, "struct"},
      {sup::dto::TypeCode::Array, "array"},     {sup::dto::TypeCode::Bool, "bool"},
      {sup::dto::TypeCode::Char8, "char8"},     {sup::dto::TypeCode::Int8, "int8"},
      {sup::dto::TypeCode::UInt8, "uint8"},     {sup::dto::TypeCode::Int16, "int16"},
      {sup::dto::TypeCode::UInt16, "uint16"},   {sup::dto::TypeCode::Int32, "int32"},
      {sup::dto::TypeCode::UInt32, "uint32"},   {sup::dto::TypeCode::Int64, "int64"},
      {sup::dto::TypeCode::UInt64, "uint64"},   {sup::dto::TypeCode::Float32, "float32"},
      {sup::dto::TypeCode::Float64, "float64"}, {sup::dto::TypeCode::String, "string"}};

  return result;
}

const std::map<sup::dto::TypeCode, std::string> type_map = CreateTypeMap();

}  // namespace

namespace anyvalueeditor
{

std::vector<std::string> GetScalarTypeNames()
{
  std::vector<sup::dto::TypeCode> scalars = {
      sup::dto::TypeCode::Bool,   sup::dto::TypeCode::Char8,   sup::dto::TypeCode::Int8,
      sup::dto::TypeCode::UInt8,  sup::dto::TypeCode::Int16,   sup::dto::TypeCode::UInt16,
      sup::dto::TypeCode::Int32,  sup::dto::TypeCode::UInt32,  sup::dto::TypeCode::Int64,
      sup::dto::TypeCode::UInt64, sup::dto::TypeCode::Float32, sup::dto::TypeCode::Float64,
      sup::dto::TypeCode::String};

  std::vector<std::string> result;
  std::transform(scalars.begin(), scalars.end(), std::back_inserter(result),
                 [](auto t)
                 {
                   auto it = type_map.find(t);
                   return it == type_map.end() ? std::string() : it->second;
                 });

  return result;
}

}  // namespace anyvalueeditor
