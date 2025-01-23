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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "job_log_viewmodel.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/jobsystem/job_log.h>
#include <oac-tree-gui/jobsystem/job_log_severity.h>

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

JobLogViewModel::JobLogViewModel(JobLog *job_log, QObject *parent)
    : QAbstractTableModel(parent), m_job_log(job_log)
{
  if (m_job_log)
  {
    m_row_count = m_job_log->GetSize();
    SetConnected(true);
  }
}

void JobLogViewModel::SetLog(JobLog *job_log)
{
  if (m_job_log == job_log)
  {
    return;
  }

  if (m_job_log)
  {
    SetConnected(false);
  }

  beginResetModel();
  m_row_count = job_log ? job_log->GetSize() : 0;
  m_job_log = job_log;
  endResetModel();

  if (m_job_log)
  {
    SetConnected(true);
  }
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
      return QString::fromStdString(ToString(record.severity));
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
  static const QStringList column_names = GetColumnNames();

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

void JobLogViewModel::OnLogEventAppended()
{
  const int current_row_count = rowCount(QModelIndex());

  if (current_row_count + 1 != m_job_log->GetSize())
  {
    throw LogicErrorException("ViewModel is out-of-sync");
  }

  beginInsertRows(QModelIndex(), current_row_count, current_row_count);
  m_row_count = m_job_log->GetSize();
  endInsertRows();
}

void JobLogViewModel::OnLogCleared()
{
  beginResetModel();
  m_row_count = 0;
  endResetModel();
}

void JobLogViewModel::SetConnected(bool value)
{
  if (!m_job_log)
  {
    throw LogicErrorException("Can't connect or disconnect non existing log");
  }

  if (value)
  {
    connect(m_job_log, &JobLog::LogEventAppended, this, &JobLogViewModel::OnLogEventAppended,
            Qt::UniqueConnection);
    connect(m_job_log, &JobLog::LogCleared, this, &JobLogViewModel::OnLogCleared,
            Qt::UniqueConnection);
  }
  else
  {
    disconnect(m_job_log, &JobLog::LogEventAppended, this, &JobLogViewModel::OnLogEventAppended);
    disconnect(m_job_log, &JobLog::LogCleared, this, &JobLogViewModel::OnLogCleared);
  }
}

}  // namespace sequencergui
