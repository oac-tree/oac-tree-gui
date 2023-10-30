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

#ifndef SEQUENCERGUI_JOBSYSTEM_JOB_LOG_H_
#define SEQUENCERGUI_JOBSYSTEM_JOB_LOG_H_

#include <sequencergui/jobsystem/log_event.h>

#include <QObject>
#include <vector>

namespace sequencergui
{

//! Holds all messages of running job in chronological order.

class JobLog : public QObject
{
  Q_OBJECT

public:
  JobLog(QObject* parent = nullptr);

  void Append(const LogEvent& log_event);

  void ClearLog();

  int GetSize() const;

  LogEvent& At(int index);

signals:
  void LogEventAppended();
  void LogCleared();

private:
  std::vector<LogEvent> m_records;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_LOG_H_
