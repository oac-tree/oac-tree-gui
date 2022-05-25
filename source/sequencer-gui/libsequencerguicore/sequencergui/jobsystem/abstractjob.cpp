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

#include "sequencergui/jobsystem/abstractjob.h"

#include "sequencergui/jobsystem/jobstates.h"

namespace sequencergui
{
AbstractJob::AbstractJob() : m_state(std::make_unique<IdleState>()) {}

AbstractJob::~AbstractJob() = default;

RunnerStatus AbstractJob::GetStatus() const
{
  return m_state->GetStatus();
}

bool AbstractJob::PerformAction(JobAction action)
{
  if (auto new_state = m_state->Handle(action, this); new_state)
  {
    SetState(std::move(new_state));
    return true;
  }

  return false;
}

void AbstractJob::SetState(std::unique_ptr<JobStateInterface> state)
{
  m_state = std::move(state);
}

}  // namespace sequencergui
