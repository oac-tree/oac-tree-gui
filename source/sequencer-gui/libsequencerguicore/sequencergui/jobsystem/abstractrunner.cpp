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

#include "sequencergui/jobsystem/abstractrunner.h"

#include <thread>

namespace sequencergui
{

struct AbstractRunner::AbstractRunnerImpl
{
  std::thread m_runner_thread;
  RunnerStatus m_runner_status;
};

AbstractRunner::AbstractRunner() : p_impl(std::make_unique<AbstractRunnerImpl>()) {}

AbstractRunner::~AbstractRunner() = default;

void AbstractRunner::Start() {}

void AbstractRunner::Stop() {}

void AbstractRunner::Pause() {}

void AbstractRunner::Step() {}

bool AbstractRunner::IsInTransition() const
{
  return false;
}

RunnerStatus AbstractRunner::GetRunnerStatus() const
{
  return p_impl->m_runner_status;
}

}  // namespace sequencergui
