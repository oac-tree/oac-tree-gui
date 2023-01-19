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

#ifndef SEQUENCERGUI_JOBSYSTEM_JOB_LOG_SEVERITY_H_
#define SEQUENCERGUI_JOBSYSTEM_JOB_LOG_SEVERITY_H_

//! @file job_log_severity.h
//! Provide log severity levels and accompanying utility functions.

#include <string>

namespace sequencergui
{

//! Defines severity levels for messages.

enum class Severity
{
  kEmergency = 0,
  kAlert,
  kCritical,
  kError,
  kWarning,
  kNotice,
  kInfo,
  kDebug,
  kTrace
};

std::string SeverityToString(Severity severity);

}  // namespace sequencergui

#endif
