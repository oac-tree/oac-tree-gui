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

//! High level actions for SequencerMonitorView.

class SequencerMonitorActions : public QObject
{
  Q_OBJECT

public:
  explicit SequencerMonitorActions(JobManager* job_manager, QObject* parent = nullptr);

  void SetJobModel(JobModel* job_model);

  void OnStartJobRequest();

  void OnPauseJobRequest();

  void OnStopJobRequest();

  void OnMakeStepRequest();

  void OnRemoveJobRequest();

private:
  JobModel* m_job_model{nullptr};
  JobManager* m_job_manager{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_SEQUENCERMONITORACTIONS_H
