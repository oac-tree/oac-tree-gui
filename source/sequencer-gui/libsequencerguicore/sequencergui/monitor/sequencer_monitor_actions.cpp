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

#include "sequencergui/monitor/sequencer_monitor_actions.h"

namespace sequencergui
{

SequencerMonitorActions::SequencerMonitorActions(JobManager *job_manager, QObject *parent)
    : QObject(parent), m_job_manager(job_manager)
{
}

void SequencerMonitorActions::SetJobModel(JobModel *job_model)
{
  m_job_model = job_model;
}

void SequencerMonitorActions::OnStartJobRequest()
{

}

void SequencerMonitorActions::OnPauseJobRequest()
{

}

void SequencerMonitorActions::OnStopJobRequest()
{

}

void SequencerMonitorActions::OnMakeStepRequest()
{

}

void SequencerMonitorActions::OnRemoveJobRequest()
{

}

}  // namespace sequencergui
