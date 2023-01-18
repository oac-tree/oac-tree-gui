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

#include "log_event.h"

namespace sequencergui
{

bool LogEvent::operator==(const LogEvent& other) const
{
  return source == other.source && severity == other.severity && date == other.date
         && time == other.time && message == other.message;
}

bool LogEvent::operator!=(const LogEvent& other) const
{
  return !(*this == other);
}

LogEvent CreateLogEvent(Severity severity, const std::string& message)
{
  return {std::string(), std::string(), severity, std::string(), message};
}

}  // namespace sequencergui
