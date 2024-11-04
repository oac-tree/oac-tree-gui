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

#ifndef SEQUENCERGUI_JOBSYSTEM_LOCAL_JOB_HANDLER_H_
#define SEQUENCERGUI_JOBSYSTEM_LOCAL_JOB_HANDLER_H_

#include <sequencergui/jobsystem/abstract_job_handler.h>
#include <sequencergui/jobsystem/user_context.h>

namespace sequencergui
{

/**
 * @brief The LocalJobHandler class is intended to run a local job represented by the JobItem.
 */
class LocalJobHandler : public AbstractJobHandler
{
  Q_OBJECT

public:
  explicit LocalJobHandler(JobItem* job_item, const UserContext& user_context = {});
  ~LocalJobHandler() override;

private:
  void OnVariableUpdatedEvent(const VariableUpdatedEvent& event) override;

  //!< dedicated listener to provide communication between domain/GUI workspace variables
  std::unique_ptr<WorkspaceItemListener> m_workspace_item_listener;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_LOCAL_JOB_HANDLER_H_
