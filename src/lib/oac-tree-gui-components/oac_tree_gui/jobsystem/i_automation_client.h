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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_JOBSYSTEM_I_AUTOMATION_CLIENT_H_
#define OAC_TREE_GUI_JOBSYSTEM_I_AUTOMATION_CLIENT_H_

#include <memory>
#include <string>

namespace oac_tree_gui
{

class AbstractJobHandler;
struct UserContext;
class RemoteJobItem;

/**
 * @brief The IAutomationClient class holds automation server connection information.
 *
 * It is used to create remote jobs. Internally holds sup::oac_tree::IJobManager.
 */
class IAutomationClient
{
public:
  virtual ~IAutomationClient() = default;

  /**
   * @brief Returns the name of the connected server.
   */
  virtual std::string GetServerName() const = 0;

  /**
   * @brief Returns number of server jobs.
   */
  virtual std::size_t GetJobCount() const = 0;

  /**
   * @brief Returns the name of the procedure for given job index.
   */
  virtual std::string GetProcedureName(std::size_t job_index) const = 0;

  /**
   * @brief Creates job handler.
   *
   * Job handler is a GUI object intended to run jobs represented by the remote job item.
   *
   * @param job_item Remote job item.
   * @param user_context The user context to handle user interactions.
   *
   * @return New AbstractJobHandler intended for ouw GUI JobManager.
   */
  virtual std::unique_ptr<AbstractJobHandler> CreateJobHandler(RemoteJobItem* job_item,
                                                               const UserContext& user_context) = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_I_AUTOMATION_CLIENT_H_
