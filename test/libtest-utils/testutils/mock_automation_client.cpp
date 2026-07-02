/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mock_automation_client.h"

#include <oac_tree_gui/jobsystem/objects/abstract_job_handler.h>

#include <utility>

namespace oac_tree_gui::test
{

AutomationClientDecorator::AutomationClientDecorator(IAutomationClient& decoratee)
    : m_decoratee(decoratee)
{
}

AutomationClientDecorator::AutomationClientDecorator(IAutomationClient& decoratee,
                                                     AutomationServerInfo server_info)
    : m_decoratee(decoratee), m_server_info(std::move(server_info))
{
}

AutomationServerInfo AutomationClientDecorator::GetServerInfo() const
{
  return m_server_info.has_value() ? m_server_info.value() : m_decoratee.GetServerInfo();
}

bool AutomationClientDecorator::IsConnected() const
{
  return m_decoratee.IsConnected();
}

std::size_t AutomationClientDecorator::GetJobCount() const
{
  return m_decoratee.GetJobCount();
}

std::string AutomationClientDecorator::GetProcedureName(std::uint32_t job_index) const
{
  return m_decoratee.GetProcedureName(job_index);
}

std::unique_ptr<AbstractJobHandler> AutomationClientDecorator::CreateJobHandler(
    RemoteJobItem* job_item, const UserContext& user_context)
{
  return m_decoratee.CreateJobHandler(job_item, user_context);
}

std::unique_ptr<IAutomationClient> CreateAutomationClientDecorator(IAutomationClient& decoratee)
{
  return std::make_unique<AutomationClientDecorator>(decoratee);
}

std::unique_ptr<IAutomationClient> CreateAutomationClientDecorator(IAutomationClient& decoratee,
                                                                   AutomationServerInfo server_info)
{
  return std::make_unique<AutomationClientDecorator>(decoratee, std::move(server_info));
}

AutomationClientFunc AutomationClientDecoratorCreateFunc(IAutomationClient& decoratee)
{
  auto result = [&decoratee](const AutomationServerInfo& server_info)
  { return CreateAutomationClientDecorator(decoratee, server_info); };

  return result;
}

AutomationClientFunc MockAutomationClientFactory::CreateFunc()
{
  return [this](const AutomationServerInfo& server_info) { return CreateClient(server_info); };
}

}  // namespace oac_tree_gui::test
