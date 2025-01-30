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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mock_automation_client.h"

#include <oac_tree_gui/jobsystem/abstract_job_handler.h>

namespace oac_tree_gui::test
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

std::unique_ptr<oac_tree_gui::AbstractJobHandler> AutomationClientDecorator::CreateJobHandler(
    oac_tree_gui::RemoteJobItem *job_item, const oac_tree_gui::UserContext &user_context)
{
  return m_decoratee.CreateJobHandler(job_item, user_context);
}

std::unique_ptr<oac_tree_gui::IAutomationClient> CreateAutomationClientDecorator(
    oac_tree_gui::IAutomationClient &decoratee)
{
  return std::make_unique<AutomationClientDecorator>(decoratee);
}

std::function<std::unique_ptr<oac_tree_gui::IAutomationClient>(const std::string &server_name)>
AutomationClientDecoratorCreateFunc(oac_tree_gui::IAutomationClient &decoratee)
{
  auto result = [&decoratee](const std::string &server_name)
  {
    (void)server_name;
    return CreateAutomationClientDecorator(decoratee);
  };

  return result;
}

}  // namespace oac_tree_gui::test
