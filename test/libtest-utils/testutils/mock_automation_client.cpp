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

#include "mock_automation_client.h"

#include <sequencergui/jobsystem/abstract_job_handler.h>

namespace testutils
{

AutomationClientDecorator::AutomationClientDecorator(IAutomationClient &decoratee)
    : m_decoratee(decoratee)
{
}

std::string AutomationClientDecorator::GetServerName() const
{
  return m_decoratee.GetServerName();
}

std::size_t AutomationClientDecorator::GetJobCount() const
{
  return m_decoratee.GetJobCount();
}

std::string AutomationClientDecorator::GetProcedureName(std::size_t job_index) const
{
  return m_decoratee.GetProcedureName(job_index);
}

std::unique_ptr<sequencergui::AbstractJobHandler> AutomationClientDecorator::CreateJobHandler(
    sequencergui::RemoteJobItem *job_item, const sequencergui::UserContext &user_context)
{
  return m_decoratee.CreateJobHandler(job_item, user_context);
}

std::unique_ptr<sequencergui::IAutomationClient> CreateAutomationClientDecorator(
    sequencergui::IAutomationClient &decoratee)
{
  return std::make_unique<AutomationClientDecorator>(decoratee);
}

std::function<std::unique_ptr<sequencergui::IAutomationClient>(const std::string &server_name)>
AutomationClientDecoratorCreateFunc(sequencergui::IAutomationClient &decoratee)
{
  auto result = [&decoratee](const std::string &server_name)
  {
    (void)server_name;
    return CreateAutomationClientDecorator(decoratee);
  };

  return result;
}

}  // namespace testutils
