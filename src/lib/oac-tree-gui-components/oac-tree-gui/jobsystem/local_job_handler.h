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

#ifndef SEQUENCERGUI_JOBSYSTEM_LOCAL_JOB_HANDLER_H_
#define SEQUENCERGUI_JOBSYSTEM_LOCAL_JOB_HANDLER_H_

#include <oac-tree-gui/jobsystem/abstract_job_handler.h>

namespace mvvm
{
class ItemListener;
}

namespace oac_tree_gui
{

class WorkspaceItemListener;
struct UserContext;

/**
 * @brief The LocalJobHandler class is intended to run a local job represented by the JobItem.
 */
class LocalJobHandler : public AbstractJobHandler
{
  Q_OBJECT

public:
  explicit LocalJobHandler(JobItem* job_item, UserContext user_context);
  ~LocalJobHandler() override;

private:
  void OnVariableUpdatedEvent(const VariableUpdatedEvent& event) override;

  /**
   * @brief Creates local domain runner.
   */
  std::unique_ptr<AbstractDomainRunner> CreateDomainRunner(UserContext user_context,
                                                           std::unique_ptr<procedure_t> procedure);

  /**
   * @brief Setup property listener to propagate tick timeouts from GUI to domain.
   *
   */
  void SetupPropertyListener();

  //!< dedicated listener to provide communication between domain/GUI workspace variables
  std::unique_ptr<WorkspaceItemListener> m_workspace_item_listener;

  //!< listens for JobItem property change
  std::unique_ptr<mvvm::ItemListener> m_property_listener;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_JOBSYSTEM_LOCAL_JOB_HANDLER_H_
