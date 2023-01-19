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

#include "job_log_viewmodel.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/monitor/job_log.h>

namespace
{
const int kColumnCount = 5;  // number of members in LogEvent
QStringList GetColumnNames()
{
  return QStringList() << "date"
                       << "time"
                       << "severity"
                       << "source"
                       << "message";
}

}  // namespace

namespace sequencergui
{

JobLogViewModel::JobLogViewModel(JobLog *job_log, QObject *parent) : m_job_log(job_log)
{
  m_row_count = m_job_log->GetSize();
}

int JobLogViewModel::rowCount(const QModelIndex &parent) const
{
  return parent.isValid() ? 0 : m_row_count;
}

int JobLogViewModel::columnCount(const QModelIndex &parent) const
{
  return parent.isValid() ? 0 : kColumnCount;
}

QVariant JobLogViewModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
  {
    return {};
  }

  if (index.row() >= m_job_log->GetSize() || index.row() < 0)
  {
    return {};
  }

  if (role == Qt::DisplayRole)
  {
    const auto &record = m_job_log->At(index.row());

    switch (index.column())
    {
    case 0:
      return QString::fromStdString(record.date);
    case 1:
      return QString::fromStdString(record.time);
    case 2:
      return QString();
    case 3:
      return QString::fromStdString(record.source);
    case 4:
      return QString::fromStdString(record.message);
    default:
      break;
    }
  }
  return {};
}

QVariant JobLogViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  static QStringList column_names = GetColumnNames();

  if (role != Qt::DisplayRole)
  {
    return {};
  }

  if (orientation == Qt::Horizontal)
  {
    return column_names.at(section);
  }

  return {};
}

Qt::ItemFlags JobLogViewModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return Qt::ItemIsEnabled;
  }

  return QAbstractTableModel::flags(index) | Qt::ItemIsSelectable;
}

//! Provides necessary ViewModel-related bookkeeping when a new LogEvent is added to a JobLog.
//!
//! This method should be called manually by someone, who knows, that LogEvent has gotten a new
//! entry. The method will generate necessary signals. Will be refactored later.

void JobLogViewModel::OnJobLogNewEntry()
{
  int current_row_count = rowCount(QModelIndex());

  if (current_row_count + 1 != m_job_log->GetSize())
  {
    throw LogicErrorException("ViewModel is out-of-sync");
  }

  beginInsertRows(QModelIndex(), current_row_count, current_row_count);
  m_row_count = m_job_log->GetSize();
  endInsertRows();
}

}  // namespace sequencergui
