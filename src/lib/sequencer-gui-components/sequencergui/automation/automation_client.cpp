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

#include "automation_client.h"

#include <sup/auto-server/automation_client.h>
#include <sup/auto-server/automation_protocol_client.h>
#include <sup/auto-server/epics_io_client.h>
#include <sup/epics/pv_access_rpc_client.h>
#include <sup/protocol/protocol_rpc_client.h>

#include <iostream>

namespace sequencergui
{

struct AutomationClient::AutomationManagerImpl
{
  sup::epics::PvAccessRPCClient pv_access_rpc_client;
  sup::protocol::ProtocolRPCClient protocol_rpc_client;
  sup::auto_server::AutomationProtocolClient auto_protocol_client;
  sup::auto_server::AnyValueIOFactoryFunction m_listener;
  sup::auto_server::AutomationClient auto_client;

  explicit AutomationManagerImpl(const std::string& server_name)
      : pv_access_rpc_client(sup::epics::GetDefaultRPCClientConfig(server_name))
      , protocol_rpc_client(pv_access_rpc_client)
      , auto_protocol_client(protocol_rpc_client)
      , m_listener(sup::auto_server::EPICSIOCLientFactoryFunction)
      , auto_client(auto_protocol_client, m_listener)
  {
  }
};

AutomationClient::AutomationClient(const std::string& server_name)
    : p_impl(std::make_unique<AutomationManagerImpl>(server_name))
{
  std::cout << p_impl->auto_protocol_client.GetServerPrefix() << " "
            << p_impl->auto_protocol_client.GetNumberOfJobs() << "\n";
}

void AutomationClient::Run(size_t job_index)
{
  p_impl->auto_protocol_client.SendJobCommand(job_index, sup::sequencer::JobCommand::kStart);
}

void AutomationClient::Pause(size_t job_index)
{
  p_impl->auto_protocol_client.SendJobCommand(job_index, sup::sequencer::JobCommand::kPause);
}

void AutomationClient::Stop(size_t job_index)
{
  p_impl->auto_protocol_client.SendJobCommand(job_index, sup::sequencer::JobCommand::kHalt);
}

void AutomationClient::Step(size_t job_index)
{
  p_impl->auto_protocol_client.SendJobCommand(job_index, sup::sequencer::JobCommand::kStep);
}

AutomationClient::~AutomationClient() = default;

size_t AutomationClient::GetJobCount() const
{
  return p_impl->auto_protocol_client.GetNumberOfJobs();
}

sup::auto_server::JobInfo AutomationClient::GetJobInfo(size_t job_index) const
{
  return p_impl->auto_protocol_client.GetJobInfo(job_index);
}

void AutomationClient::Connect(size_t job_index, sup::auto_server::IJobInfoIO* observer)
{
  p_impl->auto_client.Connect(job_index, *observer);
}

}  // namespace sequencergui
