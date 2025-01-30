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

#ifndef OAC_TREE_GUI_JOBSYSTEM_REMOTE_JOB_HANDLER_H_
#define OAC_TREE_GUI_JOBSYSTEM_REMOTE_JOB_HANDLER_H_

#include <oac-tree-gui/jobsystem/abstract_job_handler.h>
#include <oac-tree-gui/jobsystem/user_context.h>

namespace sup::oac_tree_server
{
class IJobManager;
}

namespace oac_tree_gui
{

class WorkspaceItemListener;

/**
 * @brief The RemoteJobHandler class is intended to run a remote job represented by the JobItem.
 */
class RemoteJobHandler : public AbstractJobHandler
{
  Q_OBJECT

public:
  RemoteJobHandler(JobItem* job_item, sup::oac_tree_server::IJobManager& manager, std::size_t job_index,
                   UserContext user_context);
  ~RemoteJobHandler() override;

private:
  void OnVariableUpdatedEvent(const VariableUpdatedEvent& event) override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_REMOTE_JOB_HANDLER_H_
