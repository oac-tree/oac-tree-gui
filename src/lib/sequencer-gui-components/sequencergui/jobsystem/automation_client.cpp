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

#include "automation_client.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/remote_job_handler.h>
#include <sequencergui/model/standard_job_items.h>
#include <sequencergui/jobsystem/user_context.h>

#include <sup/auto-server/epics_config_utils.h>
#include <sup/auto-server/exceptions.h>
#include <sup/sequencer/job_info.h>

namespace sequencergui
{

AutomationClient::AutomationClient(const std::string& server_name)
    : m_server_name(server_name)
    , m_automation_job_manager(sup::auto_server::utils::CreateEPICSJobManager(server_name))
{
  try
  {
    // current way to check if connection was established
    [[maybe_unused]] auto job_count = m_automation_job_manager->GetNumberOfJobs();
  }
  catch (const sup::auto_server::InvalidOperationException& ex)
  {
    throw RuntimeException("Connection to the server [" + server_name
                           + "] has failed with the message [" + ex.what() + "]");
  }
}

AutomationClient::~AutomationClient() = default;

std::string AutomationClient::GetServerName() const
{
  return m_server_name;
}

size_t AutomationClient::GetJobCount() const
{
  return m_automation_job_manager->GetNumberOfJobs();
}

std::string AutomationClient::GetProcedureName(size_t job_index) const
{
  return m_automation_job_manager->GetJobInfo(job_index).GetProcedureName();
}

std::unique_ptr<AbstractJobHandler> AutomationClient::CreateJobHandler(
    RemoteJobItem* job_item, const UserContext& user_context)
{
  auto job_index = job_item->GetRemoteJobIndex();
  return std::make_unique<RemoteJobHandler>(job_item, *m_automation_job_manager, job_index,
                                            user_context);
}

}  // namespace sequencergui
