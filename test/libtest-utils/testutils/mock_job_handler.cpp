/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "mock_job_handler.h"

namespace sequencergui::test
{

void MockJobHandler::Start()
{
  m_listener.Start(this);
}

void MockJobHandler::Pause()
{
  m_listener.Pause(this);
}

void MockJobHandler::Step()
{
  m_listener.Step(this);
}

void MockJobHandler::Stop()
{
  m_listener.Stop(this);
}

void MockJobHandler::Reset()
{
  m_listener.Reset(this);
}

bool MockJobHandler::IsRunning() const
{
  return m_listener.IsRunning(this);
}

sequencergui::RunnerStatus MockJobHandler::GetRunnerStatus() const
{
  return m_listener.GetRunnerStatus(this);
}

sequencergui::JobLog *MockJobHandler::GetJobLog() const
{
  return m_listener.GetJobLog(this);
}

void MockJobHandler::OnToggleBreakpointRequest(sequencergui::InstructionItem *instruction)
{
  m_listener.OnToggleBreakpointRequest(instruction, this);
}

sequencergui::JobItem *MockJobHandler::GetJobItem()
{
  return m_job_item;
}

sequencergui::ProcedureItem *MockJobHandler::GetExpandedProcedure() const
{
  return m_listener.GetExpandedProcedure(this);
}

}  // namespace sequencergui::test
