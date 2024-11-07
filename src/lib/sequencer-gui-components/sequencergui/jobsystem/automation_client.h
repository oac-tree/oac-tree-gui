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

#ifndef SEQUENCERGUI_JOBSYSTEM_AUTOMATION_CLIENT_H_
#define SEQUENCERGUI_JOBSYSTEM_AUTOMATION_CLIENT_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <memory>

namespace sup::auto_server
{
class IJobManager;
}

namespace sequencergui
{

class AbstractJobHandler;
class JobItem;

/**
 * @brief The AutomationClient class is a simple wrapper around automation server machinery to hide
 * its API.
 */
class AutomationClient
{
public:
  explicit AutomationClient(const std::string& server_name);
  ~AutomationClient();

  /**
   * @brief Returns number of server jobs.
   */
  size_t GetJobCount() const;

  std::unique_ptr<AbstractJobHandler> CreateJobHandler(sequencergui::JobItem* job_item,
                                                       size_t job_index);

private:
  std::unique_ptr<sup::auto_server::IJobManager> m_automation_job_manager;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_AUTOMATION_CLIENT_H_
