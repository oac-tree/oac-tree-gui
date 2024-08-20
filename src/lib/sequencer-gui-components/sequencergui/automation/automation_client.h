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

#ifndef SEQUENCERGUI_AUTOMATION_AUTOMATION_CLIENT_H_
#define SEQUENCERGUI_AUTOMATION_AUTOMATION_CLIENT_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <memory>
#include <string>

namespace sup::auto_server
{
class IJobInfoIO;
}

namespace sequencergui
{

/**
 * @brief The AutomationClient class is a simple wrapper around automation server machinery to hide
 * its API.
 */
class AutomationClient
{
public:
  explicit AutomationClient(const std::string& server_name);
  ~AutomationClient();

  void Run(size_t job_index);

  void Pause(size_t job_index);

  void Stop(size_t job_index);

  void Step(size_t job_index);

  /**
   * @brief Returns number of server jobs.
   */
  size_t GetJobCount() const;

  /**
   * @brief Returns full job information for given job index.
   */
  sup::auto_server::JobInfo GetJobInfo(size_t job_index) const;

  /**
   * @brief Connect job observer with remote job with the given index.
   */
  void Connect(size_t job_index, sup::auto_server::IJobInfoIO *observer);

private:
  struct AutomationManagerImpl;
  std::unique_ptr<AutomationManagerImpl> p_impl;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_AUTOMATION_AUTOMATION_CLIENT_H_
