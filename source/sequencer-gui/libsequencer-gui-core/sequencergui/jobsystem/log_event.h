/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_LOG_EVENT_H_
#define SEQUENCERGUI_JOBSYSTEM_LOG_EVENT_H_

#include <sequencergui/jobsystem/job_log_severity.h>

#include <string>

namespace sequencergui
{

//! Represents a log event during sequencer procedure execution.
struct LogEvent
{
  std::string source;                     //!< source of the message
  Severity severity = Severity::kNotice;  //!< log message severity level
  std::string date;                       //!< date of the message in the format yyyy-mm-dd
  std::string time;                       //!< time of the message in the format hh:mm:ss.zzz
  std::string message;                    //!< text of the message

  bool operator==(const LogEvent& other) const;
  bool operator!=(const LogEvent& other) const;
};

//! Creates log event for the current moment of time with given severity level and message text.
LogEvent CreateLogEvent(Severity severity, const std::string& message);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_UTILS_H_
