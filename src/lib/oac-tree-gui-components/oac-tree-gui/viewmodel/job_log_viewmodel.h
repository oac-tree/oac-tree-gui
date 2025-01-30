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

#ifndef OAC_TREE_GUI_VIEWMODEL_JOB_LOG_VIEWMODEL_H_
#define OAC_TREE_GUI_VIEWMODEL_JOB_LOG_VIEWMODEL_H_

#include <QAbstractTableModel>

namespace oac_tree_gui
{

class JobLog;

/**
 * @brief The JobLogViewModel class is a viewmodel to show JobLog content in the form of the
 * table.
 */
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
  /**
   * @brief Provides necessary view model bookkeeping when a new LogEvent is added to a JobLog.
   *
   * This method should be connected with JobLog::LogEventAppended.
   */
  void OnLogEventAppended();

  /**
   * @brief Clears the model.
   *
   * Called when log is cleared.
   */
  void OnLogCleared();

  /**
   * @brief Set model connected/disconnected with JobLog events.
   */
  void SetConnected(bool value);

  //!< current container with LogEvents
  JobLog *m_job_log{nullptr};

  //!< Number of rows shown by the table. May differ from the actual number of LogEvents in
  //!< the container.
  int m_row_count{0};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_JOB_LOG_VIEWMODEL_H_
