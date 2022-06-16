/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_SEQUENCERMONITORACTIONS_H
#define SEQUENCERGUI_MONITOR_SEQUENCERMONITORACTIONS_H

#include <QObject>
#include <functional>

namespace sequencergui
{

class JobModel;
class JobManager;
class JobItem;
class ProcedureItem;

//! High level actions for SequencerMonitorView.
//! Provides reporting on exception throw.

class SequencerMonitorActions : public QObject
{
  Q_OBJECT

public:
  //!< A callback to get currently selected JobItem.
  using selection_callback_t = std::function<JobItem*()>;

  explicit SequencerMonitorActions(JobManager* job_manager, selection_callback_t selection_callback,
                                   QObject* parent = nullptr);

  void SetJobModel(JobModel* job_model);

  void OnSubmitJobRequest(ProcedureItem* procedure_item);

  void OnStartJobRequest();

  void OnPauseJobRequest();

  void OnStopJobRequest();

  void OnMakeStepRequest();

  void OnRemoveJobRequest();

private:
  void CheckConditions();

  JobModel* m_job_model{nullptr};
  JobManager* m_job_manager{nullptr};
  selection_callback_t m_job_selection_callback;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_SEQUENCERMONITORACTIONS_H
