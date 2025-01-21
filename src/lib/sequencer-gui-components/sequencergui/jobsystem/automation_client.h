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

#ifndef SEQUENCERGUI_JOBSYSTEM_AUTOMATION_CLIENT_H_
#define SEQUENCERGUI_JOBSYSTEM_AUTOMATION_CLIENT_H_

#include "sequencergui/jobsystem/user_context.h"

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/i_automation_client.h>

#include <memory>

namespace sup::auto_server
{
class IJobManager;
}

namespace sequencergui
{

class AbstractJobHandler;

/**
 * @brief The AutomationClient class is a simple wrapper around automation server machinery to hide
 * its API.
 */
class AutomationClient : public IAutomationClient
{
public:
  explicit AutomationClient(const std::string& server_name);
  ~AutomationClient() override;

  std::string GetServerName() const override;

  std::size_t GetJobCount() const override;

  std::string GetProcedureName(std::size_t job_index) const override;

  std::unique_ptr<AbstractJobHandler> CreateJobHandler(RemoteJobItem* job_item,
                                                       const UserContext& user_context) override;

private:
  std::string m_server_name;
  std::unique_ptr<sup::auto_server::IJobManager> m_automation_job_manager;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_AUTOMATION_CLIENT_H_
