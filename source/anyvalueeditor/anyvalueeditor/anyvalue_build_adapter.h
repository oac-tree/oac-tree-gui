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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILD_ADAPTER_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILD_ADAPTER_H_

#include <sup/dto/basic_scalar_types.h>

#include <memory>
#include <string>

namespace sup::dto
{
class AnyValue;
}

namespace anyvalueeditor
{

//! Builds AnyValue in step-wise manner by calling methods to add fields and structs.

class AnyValueBuildAdapter
{
public:
  AnyValueBuildAdapter();
  ~AnyValueBuildAdapter();

  sup::dto::AnyValue MoveAnyValue() const;

  void Bool(const std::string& member_name, ::sup::dto::boolean value);
  void Int8(const std::string& member_name, ::sup::dto::int8 value);
  void UInt8(const std::string& member_name, ::sup::dto::uint8 value);
  void Int16(const std::string& member_name, ::sup::dto::int16 value);
  void UInt16(const std::string& member_name, ::sup::dto::uint16 value);
  void Int32(const std::string& member_name, ::sup::dto::int32 value);
  void UInt32(const std::string& member_name, ::sup::dto::uint32 value);
  void Int64(const std::string& member_name, ::sup::dto::int64 value);
  void UInt64(const std::string& member_name, ::sup::dto::uint64 value);
  void Float32(const std::string& member_name, ::sup::dto::float32 value);
  void Float64(const std::string& member_name, ::sup::dto::float64 value);
  void String(const std::string& member_name, const std::string& value);

  void AddMember(const std::string& member_name, const ::sup::dto::AnyValue& value);

  void StartStruct(const std::string& struct_name = {});

  void EndStruct(const std::string& member_name = {});

private:
  struct AnyValueBuildAdapterImpl;
  std::unique_ptr<AnyValueBuildAdapterImpl> p_impl;
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILD_ADAPTER_H_
