/******************************************************************************
 *
 * Project       : Supervision and automation system EPICS interface
 *
 * Description   : Library of SUP components for EPICS network protocol
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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILD_ADAPTER_V2_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILD_ADAPTER_V2_H_

#include <anyvalueeditor/dto_types_fwd.h>
#include <sup/dto/basic_scalar_types.h>

#include <memory>
#include <string>

namespace anyvalueeditor
{

//! Builds AnyValue in step-wise manner by calling methods to add fields and structs.

class AnyValueBuildAdapterV2
{
public:
  AnyValueBuildAdapterV2();
  ~AnyValueBuildAdapterV2();

  sup::dto::AnyValue MoveAnyValue() const;

  void Bool(::sup::dto::boolean value);
  void Int8(::sup::dto::int8 value);
  void UInt8(::sup::dto::uint8 value);
  void Int16(::sup::dto::int16 value);
  void UInt16(::sup::dto::uint16 value);
  void Int32(::sup::dto::int32 value);
  void UInt32(::sup::dto::uint32 value);
  void Int64(::sup::dto::int64 value);
  void UInt64(::sup::dto::uint64 value);
  void Float32(::sup::dto::float32 value);
  void Float64(::sup::dto::float64 value);
  void String(const std::string& value);

  void AddValue(sup::dto::AnyValue anyvalue);

  void StartStruct(const std::string& struct_name = {});

  void EndStruct();

  void StartField(const std::string& field_name);

  void EndField();

  void AddMember(const std::string& name, sup::dto::AnyValue anyvalue);

  void StartArray(const std::string& array_name = {});

  void StartArrayElement();

  void EndArrayElement();

  void AndArrayElement(sup::dto::AnyValue anyvalue);

  void EndArray();

  int GetStackSize() const;

private:
  struct AnyValueBuildAdapterV2Impl;
  std::unique_ptr<AnyValueBuildAdapterV2Impl> p_impl;
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILD_ADAPTER_V2_H_
