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

#include "remote_job_observer.h"

#include <iostream>

namespace sequencergui
{

void RemoteJobObserver::InitNumberOfInstructions(sup::dto::uint32 n_instr)
{
  std::cout << "RemoteJobObserver::InitNumberOfInstructions";
}

void RemoteJobObserver::InstructionStateUpdated(sup::dto::uint32 instr_idx,
                                                sup::auto_server::InstructionState state)
{
  std::cout << "RemoteJobObserver::InstructionStateUpdated";
}

void RemoteJobObserver::VariableUpdated(sup::dto::uint32 var_idx, const sup::dto::AnyValue &value,
                                        bool connected)
{
  std::cout << "RemoteJobObserver::VariableUpdated";
}

void RemoteJobObserver::JobStateUpdated(sup::sequencer::JobState state)
{
  std::cout << "RemoteJobObserver::JobStateUpdated";
}

bool RemoteJobObserver::PutValue(const sup::dto::AnyValue &value, const std::string &description)
{
  std::cout << "RemoteJobObserver::PutValue";
  return false;
}

bool RemoteJobObserver::GetUserValue(sup::dto::AnyValue &value, const std::string &description)
{
  std::cout << "RemoteJobObserver::GetUserValue";
  return false;
}

int RemoteJobObserver::GetUserChoice(const std::vector<std::string> &options,
                                     const sup::dto::AnyValue &metadata)
{
  std::cout << "RemoteJobObserver::GetUserChoice";
  return 0;
}

void RemoteJobObserver::Message(const std::string &message)
{
  std::cout << "RemoteJobObserver::Message";
}

void RemoteJobObserver::Log(int severity, const std::string &message)
{
  std::cout << "RemoteJobObserver::Log";
}

}  // namespace sequencergui
