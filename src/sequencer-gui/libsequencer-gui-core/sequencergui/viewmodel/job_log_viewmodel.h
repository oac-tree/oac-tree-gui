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

#ifndef SEQUENCERGUI_VIEWMODEL_JOB_LOG_VIEWMODEL_H_
#define SEQUENCERGUI_VIEWMODEL_JOB_LOG_VIEWMODEL_H_

#include <QAbstractTableModel>

namespace sequencergui
{

class JobLog;

//! View model to represent JobLog in Qt tree in a form of a table.

class JobLogViewModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit JobLogViewModel(JobLog *job_log, QObject *parent = nullptr);

  void SetLog(JobLog *job_log);

  int rowCount(const QModelIndex &parent) const override;

  int columnCount(const QModelIndex &parent) const override;

  QVariant data(const QModelIndex &index, int role) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
  void OnLogEventAppended();

  void OnLogCleared();

private:
  void SetConnected(bool value);

  //!< current container with LogEvents
  JobLog *m_job_log{nullptr};

  //!< Number of rows shown by the table table. May differ from the actual number of LogEvents in
  //!< the container.
  int m_row_count{0};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_JOB_LOG_VIEWMODEL_H_
