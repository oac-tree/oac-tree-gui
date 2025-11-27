/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "job_log_severity.h"

#include <oac_tree_gui/core/exceptions.h>

#include <map>

namespace
{
constexpr auto kEmergencyString = "EMERGENCY";
constexpr auto kAlertString = "ALERT";
constexpr auto kCriticalString = "CRITICAL";
constexpr auto kErrorString = "ERROR";
constexpr auto kWarningString = "WARNING";
constexpr auto kNoticeString = "NOTICE";
constexpr auto kInfoString = "INFO";
constexpr auto kDebugString = "DEBUG";
constexpr auto kTraceString = "TRACE";

std::map<oac_tree_gui::Severity, std::string> CreateSeverityMap()
{
  std::map<oac_tree_gui::Severity, std::string> result = {
      {oac_tree_gui::Severity::kEmergency, kEmergencyString},
      {oac_tree_gui::Severity::kAlert, kAlertString},
      {oac_tree_gui::Severity::kCritical, kCriticalString},
      {oac_tree_gui::Severity::kError, kErrorString},
      {oac_tree_gui::Severity::kWarning, kWarningString},
      {oac_tree_gui::Severity::kNotice, kNoticeString},
      {oac_tree_gui::Severity::kInfo, kInfoString},
      {oac_tree_gui::Severity::kDebug, kDebugString},
      {oac_tree_gui::Severity::kTrace, kTraceString}};
  return result;
}

}  // namespace

namespace oac_tree_gui
{

std::string ToString(Severity severity)
{
  static const auto severity_map = CreateSeverityMap();
  auto iter = severity_map.find(severity);

  if (iter == severity_map.end())
  {
    throw RuntimeException("No severity found");
  }

  return iter->second;
}

}  // namespace oac_tree_gui
