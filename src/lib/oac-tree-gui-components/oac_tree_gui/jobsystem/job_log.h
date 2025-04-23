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

#ifndef OAC_TREE_GUI_JOBSYSTEM_JOB_LOG_H_
#define OAC_TREE_GUI_JOBSYSTEM_JOB_LOG_H_

#include <oac_tree_gui/jobsystem/log_event.h>

#include <QObject>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The JobLog class holds all messages of running job in chronological order.
 */
class JobLog : public QObject
{
  Q_OBJECT

public:
  explicit JobLog(QObject* parent_object = nullptr);

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

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_JOB_LOG_H_
