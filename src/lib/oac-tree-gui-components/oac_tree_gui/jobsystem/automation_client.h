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

#ifndef OAC_TREE_GUI_JOBSYSTEM_AUTOMATION_CLIENT_H_
#define OAC_TREE_GUI_JOBSYSTEM_AUTOMATION_CLIENT_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/jobsystem/i_automation_client.h>

#include <memory>

#include "oac_tree_gui/jobsystem/user_context.h"

namespace sup::oac_tree_server
{
class IJobManager;
}

namespace oac_tree_gui
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
  std::unique_ptr<sup::oac_tree_server::IJobManager> m_automation_job_manager;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_AUTOMATION_CLIENT_H_
