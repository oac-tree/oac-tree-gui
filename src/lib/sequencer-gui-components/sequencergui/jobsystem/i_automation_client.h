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

#ifndef SEQUENCERGUI_JOBSYSTEM_I_AUTOMATION_CLIENT_H_
#define SEQUENCERGUI_JOBSYSTEM_I_AUTOMATION_CLIENT_H_

#include <memory>
#include <string>

namespace sequencergui
{

class AbstractJobHandler;
struct UserContext;
class RemoteJobItem;

/**
 * @brief The IAutomationClient class holds automation server connection information.
 *
 * It is used to create remote jobs. Internally holds sup::sequencer::IJobManager.
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_I_AUTOMATION_CLIENT_H_
