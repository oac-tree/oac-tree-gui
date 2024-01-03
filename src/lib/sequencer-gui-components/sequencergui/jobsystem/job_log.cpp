/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "job_log.h"

namespace sequencergui
{

JobLog::JobLog(QObject* parent) : QObject(parent) {}

void JobLog::Append(const LogEvent& log_event)
{
  m_records.push_back(log_event);
  emit LogEventAppended();
}

void JobLog::ClearLog()
{
  m_records.clear();
  emit LogCleared();
}

int JobLog::GetSize() const
{
  return static_cast<int>(m_records.size());
}

LogEvent& JobLog::At(int index)
{
  return m_records.at(index);
}

};  // namespace sequencergui
