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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mock_job_handler.h"

namespace oac_tree_gui::test
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

oac_tree_gui::RunnerStatus MockJobHandler::GetRunnerStatus() const
{
  return m_listener.GetRunnerStatus(this);
}

oac_tree_gui::JobLog *MockJobHandler::GetJobLog() const
{
  return m_listener.GetJobLog(this);
}

void MockJobHandler::OnToggleBreakpointRequest(oac_tree_gui::InstructionItem *instruction)
{
  m_listener.OnToggleBreakpointRequest(instruction, this);
}

oac_tree_gui::JobItem *MockJobHandler::GetJobItem()
{
  return m_job_item;
}

oac_tree_gui::ProcedureItem *MockJobHandler::GetExpandedProcedure() const
{
  return m_listener.GetExpandedProcedure(this);
}

}  // namespace oac_tree_gui::test
