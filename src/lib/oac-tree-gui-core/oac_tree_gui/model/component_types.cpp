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

#include <iomanip>

namespace oac_tree_gui
{

bool operator==(const ProcedureEditorInfo& lhs, const ProcedureEditorInfo& rhs)
{
  return (lhs.procedure_id == rhs.procedure_id) && (lhs.editor_type == rhs.editor_type);
}

bool operator!=(const ProcedureEditorInfo& lhs, const ProcedureEditorInfo& rhs)
{
  return !(lhs == rhs);
}

std::string GetStringFromProcedureInfo(const ProcedureEditorInfo& info)
{
  std::ostringstream stream;
  stream << std::quoted(info.procedure_id) << ' ' << static_cast<std::uint32_t>(info.editor_type);
  return stream.str();
}

ProcedureEditorInfo GetProcedureInfoFromString(const std::string& str)
{
  std::istringstream stream(str);
  std::string procedure_id;
  std::uint32_t tab_as_int = 0;
  stream >> std::quoted(procedure_id) >> tab_as_int;
  return ProcedureEditorInfo{procedure_id, static_cast<ProcedureEditorType>(tab_as_int)};
}

bool operator==(const ComposerViewInfo& lhs, const ComposerViewInfo& rhs)
{
  return (lhs.splitter_state == rhs.splitter_state)
         && (lhs.editor_info_list == rhs.editor_info_list);
}

bool operator!=(const ComposerViewInfo& lhs, const ComposerViewInfo& rhs)
{
  return !(lhs == rhs);
}

}  // namespace oac_tree_gui
