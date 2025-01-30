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

#ifndef SEQUENCERGUI_PVMONITOR_WORKSPACE_SYNCHRONIZER_H_
#define SEQUENCERGUI_PVMONITOR_WORKSPACE_SYNCHRONIZER_H_

#include <QObject>
#include <memory>

namespace sup::oac_tree
{
class Workspace;
}  // namespace sup::oac_tree

namespace oac_tree_gui
{

class WorkspaceItem;
class DomainWorkspaceListener;
class WorkspaceItemListener;
class VariableUpdatedEvent;

/**
 * @brief The WorkspaceSynchronizer class provides data synchronization between WorkspaceItem and
 * sequencer Workspace.
 *
 * All AnyValue updates in domain Workspace will be propagated to WorkspaceItem. Similarly, all
 * DataChangedEvent on GUI model side will be propagated to oac-tree Workspace. The number of
 * variables in both workspaces, as well as their names, should coincide.
 *
 * Domain workspace should outlive WorkspaceSynchronizer.
 */
class WorkspaceSynchronizer
{
public:
  /**
   * @brief Main c-tor when domain workspace and GUI workspace are known upfront.
   */
  WorkspaceSynchronizer(WorkspaceItem* workspace_item, sup::oac_tree::Workspace* domain_workspace);

  ~WorkspaceSynchronizer();

  /**
   * @brief Checks if domain queue is empty.
   *
   * To test if all queued connection events from the domain has ended up in a GUI. For testing
   * purposes.
   */
  bool IsEmptyQueue() const;

private:
  /**
   * @brief Returns a callback to update WorkspaceItem.
   */
  std::function<void(const VariableUpdatedEvent& event)> CreateUpdateGUICallback() const;

  WorkspaceItem* m_workspace_item{nullptr};
  sup::oac_tree::Workspace* m_domain_workspace{nullptr};
  std::unique_ptr<DomainWorkspaceListener> m_domain_workspace_listener;
  std::unique_ptr<WorkspaceItemListener> m_workspace_item_listener;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_SYNCHRONIZER_H_
