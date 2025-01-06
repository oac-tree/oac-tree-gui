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

#ifndef SEQUENCERGUI_JOBSYSTEM_REMOTE_JOB_HANDLER_H_
#define SEQUENCERGUI_JOBSYSTEM_REMOTE_JOB_HANDLER_H_

#include <sequencergui/jobsystem/abstract_job_handler.h>
#include <sequencergui/jobsystem/user_context.h>

namespace sup::auto_server
{
class IJobManager;
}

namespace sequencergui
{

class WorkspaceItemListener;

/**
 * @brief The RemoteJobHandler class is intended to run a remote job represented by the JobItem.
 */
class RemoteJobHandler : public AbstractJobHandler
{
  Q_OBJECT

public:
  RemoteJobHandler(JobItem* job_item, sup::auto_server::IJobManager& manager, size_t job_index,
                   UserContext user_context);
  ~RemoteJobHandler() override;

private:
  void OnVariableUpdatedEvent(const VariableUpdatedEvent& event) override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_REMOTE_JOB_HANDLER_H_
