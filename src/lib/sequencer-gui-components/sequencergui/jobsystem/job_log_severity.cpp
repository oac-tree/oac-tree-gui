/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "job_log_severity.h"

#include <sequencergui/core/exceptions.h>

#include <map>

namespace
{
const char* kEmergencyString = "EMERGENCY";
const char* kAlertString = "ALERT";
const char* kCriticalString = "CRITICAL";
const char* kErrorString = "ERROR";
const char* kWarningString = "WARNING";
const char* kNoticeString = "NOTICE";
const char* kInfoString = "INFO";
const char* kDebugString = "DEBUG";
const char* kTraceString = "TRACE";

std::map<sequencergui::Severity, std::string> CreateSeverityMap()
{
  std::map<sequencergui::Severity, std::string> result = {
      {sequencergui::Severity::kEmergency, kEmergencyString},
      {sequencergui::Severity::kAlert, kAlertString},
      {sequencergui::Severity::kCritical, kCriticalString},
      {sequencergui::Severity::kError, kErrorString},
      {sequencergui::Severity::kWarning, kWarningString},
      {sequencergui::Severity::kNotice, kNoticeString},
      {sequencergui::Severity::kInfo, kInfoString},
      {sequencergui::Severity::kDebug, kDebugString},
      {sequencergui::Severity::kTrace, kTraceString}};
  return result;
}

}  // namespace

namespace sequencergui
{

std::string ToString(Severity severity)
{
  static const auto severity_map = CreateSeverityMap();
  auto iter = severity_map.find(severity);

  if (iter == severity_map.end())
  {
    throw NotFoundKeyException("No severity found");
  }

  return iter->second;
}

}  // namespace sequencergui
