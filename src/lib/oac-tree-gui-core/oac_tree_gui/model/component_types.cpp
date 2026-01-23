/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "component_types.h"

#include <mvvm/utils/string_utils.h>

namespace oac_tree_gui
{
namespace
{

constexpr auto kSeparator = ";";
constexpr auto kIdentifierKey = "identifiers";
constexpr auto kTabKey = "tabs";
}  // namespace

bool operator==(const ProcedureEditorInfo& lhs, const ProcedureEditorInfo& rhs)
{
  return (lhs.procedure_id == rhs.procedure_id) && (lhs.editor_type == rhs.editor_type);
}

bool operator!=(const ProcedureEditorInfo& lhs, const ProcedureEditorInfo& rhs)
{
  return !(lhs == rhs);
}

std::vector<ProcedureEditorInfo> GetProcedureInfoListFromString(const std::string& str)
{
  auto optional_identifier_string = mvvm::utils::GetKeyContent(str, std::string(kIdentifierKey));
  auto optional_tab_string = mvvm::utils::GetKeyContent(str, std::string(kTabKey));
  if (!optional_identifier_string.has_value() || !optional_tab_string.has_value())
  {
    return {};
  }
  const auto identifier_string = optional_identifier_string.value();
  const auto tab_string = optional_tab_string.value();
  const auto identifiers = mvvm::utils::SplitString(identifier_string, std::string(kSeparator));
  const auto tab_strings = mvvm::utils::SplitString(tab_string, std::string(kSeparator));
  if (identifiers.size() != tab_strings.size())
  {
    return {};
  }
  std::vector<ProcedureEditorInfo> result;
  for (std::size_t i = 0; i < identifiers.size(); ++i)
  {
    ProcedureEditorInfo info;
    info.procedure_id = identifiers[i];
    info.editor_type = static_cast<ProcedureEditorType>(std::stoul(tab_strings[i], nullptr, 10));
    result.push_back(info);
  }
  return result;
}

std::string GetStringFromProcedureInfoList(const std::vector<ProcedureEditorInfo>& info_list)
{
  std::vector<std::string> identifiers;
  std::vector<std::uint32_t> tabs;
  for (const auto& info : info_list)
  {
    identifiers.push_back(info.procedure_id);
    tabs.push_back(static_cast<std::uint32_t>(info.editor_type));
  }

  std::stringstream result;
  result << kIdentifierKey << "["
         << mvvm::utils::VectorToString(identifiers, std::string(kSeparator)) << "]";
  result << " " << kTabKey << "["
         << mvvm::utils::VectorToString(tabs, std::string(kSeparator)) << "]";
  return result.str();
}

}  // namespace oac_tree_gui
