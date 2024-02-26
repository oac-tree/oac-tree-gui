/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "domain_runner.h"

namespace sequencergui::experimental
{

bool DomainRunner::Start()
{
  if (!IsReady())
  {
    return false;
  }

  m_runner_state = kStartPressed;
  return true;
}

bool DomainRunner::Stop()
{
  if (!IsReady())
  {
    return false;
  }

  m_runner_state = kStopPressed;
  return true;
}

bool DomainRunner::Pause()
{
  if (!IsReady())
  {
    return false;
  }

  m_runner_state = kPausePressed;
  return true;
}

bool DomainRunner::Step()
{
  if (!IsReady())
  {
    return false;
  }

  m_runner_state = kStepPressed;
  return true;
}

DomainRunner::RunnerState DomainRunner::GetRunnerState() const
{
  return m_runner_state;
}

bool DomainRunner::IsReady()
{
  return m_runner_state == kReady;
}

}  // namespace sequencergui::experimental
