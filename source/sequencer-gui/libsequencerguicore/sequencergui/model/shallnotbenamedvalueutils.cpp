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

#include "sequencergui/model/shallnotbenamedvalueutils.h"

#include "Variable.h"

#include <common/AnyValueHelper.h>

namespace
{
// FIXME the code is borrowed from CLIInterface.cpp
// Remove duplication

using ParseFunction = bool (*)(::ccs::types::AnyValue &value, const std::string &str);

template <typename T>
bool ParserFunctionT(::ccs::types::AnyValue &value, const std::string &str)
{
  std::istringstream istr(str);
  T val;
  istr >> val;
  if (istr.fail())
  {
    log_warning("ParseStringToScalarAnyvalue() - could not parse ('%s') in type ('%s)", str.c_str(),
                value.GetType()->GetName());
    return false;
  }
  value = val;
  return true;
}

/**
 * @todo Extend possible input values: currently only 'true' or 'false'. Possible alternatives
 * are integers (zero is false), case insensitive true/false, yes/no, etc.
 */
template <>
bool ParserFunctionT<::ccs::types::boolean>(::ccs::types::AnyValue &value, const std::string &str)
{
  std::istringstream istr(str);
  ::ccs::types::boolean val;
  istr >> std::boolalpha >> val;
  if (istr.fail())
  {
    log_warning("ParseStringToScalarAnyvalue() - could not parse ('%s') in type ('%s)", str.c_str(),
                value.GetType()->GetName());
    return false;
  }
  value = val;
  return true;
}

template <>
bool ParserFunctionT<::ccs::types::string>(::ccs::types::AnyValue &value, const std::string &str)
{
  ::ccs::types::string buffer;
  ::ccs::HelperTools::SafeStringCopy(buffer, str.c_str(), 64);
  value = buffer;
  return true;
}

std::map<std::string, ParseFunction> CreateParserMap()
{
  std::map<std::string, ParseFunction> parser_map;
  parser_map["bool"] = ParserFunctionT<::ccs::types::boolean>;
  parser_map["char8"] = ParserFunctionT<::ccs::types::char8>;
  parser_map["int8"] = ParserFunctionT<::ccs::types::int8>;
  parser_map["uint8"] = ParserFunctionT<::ccs::types::uint8>;
  parser_map["int16"] = ParserFunctionT<::ccs::types::int16>;
  parser_map["uint16"] = ParserFunctionT<::ccs::types::uint16>;
  parser_map["int32"] = ParserFunctionT<::ccs::types::int32>;
  parser_map["uint32"] = ParserFunctionT<::ccs::types::uint32>;
  parser_map["uint64"] = ParserFunctionT<::ccs::types::uint64>;
  parser_map["float32"] = ParserFunctionT<::ccs::types::float32>;
  parser_map["float64"] = ParserFunctionT<::ccs::types::float64>;
  parser_map["string"] = ParserFunctionT<::ccs::types::string>;
  return parser_map;
}

std::map<std::string, ParseFunction> &GetParserMap()
{
  static std::map<std::string, ParseFunction> parser_map = CreateParserMap();
  return parser_map;
}

}  // namespace

namespace sequencergui::DomainUtils
{

std::string GetJsonString(const anyvalue_t *value)
{
  ccs::types::char8 buffer[4096] = STRING_UNDEFINED;
  auto result = value->SerialiseInstance(buffer, 4096u);
  return std::string(buffer);
}

std::string GetValueJsonString(const variable_t *value)
{
  ::ccs::types::AnyValue anyvalue;
  value->GetValue(anyvalue);
  return GetJsonString(&anyvalue);
}

bool ParseStringToScalarAnyvalue(const std::string &str, anyvalue_t &value)
{
  std::string type_name = value.GetType()->GetName();

  auto &parser_map = GetParserMap();
  if (parser_map.find(type_name) == parser_map.end())
  {
    return false;
  }
  auto parse_function = parser_map[type_name];
  return parse_function(value, str);
}

}  // namespace sequi::DomainUtils
