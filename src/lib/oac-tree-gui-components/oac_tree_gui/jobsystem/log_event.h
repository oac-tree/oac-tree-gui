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

#ifndef OAC_TREE_GUI_JOBSYSTEM_LOG_EVENT_H_
#define OAC_TREE_GUI_JOBSYSTEM_LOG_EVENT_H_

#include <oac_tree_gui/jobsystem/job_log_severity.h>

#include <string>

namespace oac_tree_gui
{

//! Represents a log event during sequencer procedure execution.
struct LogEvent
{
  std::string date;                       //!< date of the message in the format yyyy-mm-dd
  std::string time;                       //!< time of the message in the format hh:mm:ss.zzz
  Severity severity = Severity::kNotice;  //!< log message severity level
  std::string source;                     //!< source of the message
  std::string message;                    //!< text of the message

  bool operator==(const LogEvent& other) const;
  bool operator!=(const LogEvent& other) const;
};

//! Creates log event for the current moment of time with a given severity level and message text.
LogEvent CreateLogEvent(Severity severity, const std::string& message);

//! Returns format used for the date accross the wphole app.
//! For the moment using "yyyy.MM.dd".
std::string GetLogEventDateFormat();

//! Returns format used for the time accross the whole app.
//! For the moment using "hh:mm:ss.zzz".
std::string GetLogEventTimeFormat();

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_LOG_EVENT_H_
