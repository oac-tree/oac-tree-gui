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

#include <sup/auto-server/client_job.h>
#include <sup/auto-server/epics_client_utils.h>
#include <sup/protocol/protocol_rpc_client.h>

#include <iostream>

namespace sequencergui
{

struct AutomationClient::AutomationManagerImpl
{
  std::unique_ptr<sup::auto_server::IJobManager> automation_client;
  std::unique_ptr<sup::sequencer::IJob> client_job;

  explicit AutomationManagerImpl(const std::string& server_name)
      : automation_client(sup::auto_server::utils::CreateEPICSJobManager(server_name))
  {
  }
};

AutomationClient::AutomationClient(const std::string& server_name)
    : p_impl(std::make_unique<AutomationManagerImpl>(server_name))
{
  std::cout << "Number of jobs " << p_impl->automation_client->GetNumberOfJobs() << "\n";
}

void AutomationClient::Run(size_t job_index)
{
  (void)job_index;
  p_impl->client_job->Start();
}

void AutomationClient::Pause(size_t job_index)
{
  (void)job_index;
  p_impl->client_job->Pause();
}

void AutomationClient::Stop(size_t job_index)
{
  (void)job_index;
  p_impl->client_job->Halt();
}

void AutomationClient::Step(size_t job_index)
{
  (void)job_index;
  p_impl->client_job->Step();
}

AutomationClient::~AutomationClient() = default;

size_t AutomationClient::GetJobCount() const
{
  return p_impl->automation_client->GetNumberOfJobs();
}

sup::sequencer::JobInfo AutomationClient::GetJobInfo(size_t job_index) const
{
  (void)job_index;
  return p_impl->client_job->GetInfo();
}

void AutomationClient::Connect(size_t job_index, sup::sequencer::IJobInfoIO* observer)
{
  (void)job_index;

  p_impl->client_job = sup::auto_server::CreateClientJob(
      *p_impl->automation_client, 0, sup::auto_server::utils::CreateEPICSIOClient, *observer);
}

}  // namespace sequencergui
