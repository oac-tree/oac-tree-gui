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

#ifndef SEQUENCERGUI_PVMONITOR_WORKSPACE_SYNCHRONIZER_H_
#define SEQUENCERGUI_PVMONITOR_WORKSPACE_SYNCHRONIZER_H_

#include <QObject>
#include <memory>

namespace sup::sequencer
{
class Workspace;
}  // namespace sup::sequencer

namespace sequencergui
{

class WorkspaceItem;
class SequencerWorkspaceListener;
class WorkspaceItemController;
class WorkspaceEvent;

/**
 * @brief The WorkspaceSynchronizer class provides data synchronization between WorkspaceItem and
 * sequencer Workspace.
 *
 * All AnyValue updates in domain Workspace will be propagated to WorkspaceItem. Similarly, all
 * DataChangedEvent on GUI model side will be propagated to Sequencer Workspace. The number of
 * variables in both workspaces, as well as their names, should coincide. There are two ways to use
 * the controller.
 *
 * @code
    // When the domain and the GUI workspaces are known upfront:
    WorkspaceSynchronizer synchronizer(workspace, workspace_item);
    syncronizer->Start();

    // When the GUI workspace was generated after the domain worspace.
    WorkspaceSynchronizer synchronizer(workspace);
    workspace->Setup();
    synchronizer->SetWorkspaceItem(workspace_item);
    synchronizer->Start();
 * @endcode
 */
class WorkspaceSynchronizer : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor when domain workspace is known upfront.
   *
   * @details This c-tor requires that the WorkspaceItem should be set after.
   */
  explicit WorkspaceSynchronizer(sup::sequencer::Workspace* domain_workspace,
                                 QObject* parent = nullptr);

  /**
   * @brief Main c-tor when domain workspace and GUI workspace are known upfront.
   */
  WorkspaceSynchronizer(WorkspaceItem* workspace_item, sup::sequencer::Workspace* domain_workspace,
                        QObject* parent = nullptr);

  ~WorkspaceSynchronizer() override;

  void SetWorkspaceItem(WorkspaceItem* workspace_item);

  /**
   * @brief Returns true if syncronizer has been already started.
   */
  bool HasStarted() const;

  /**
   * @brief Start domain/GUI workspace syncronization.
   */
  void Start();

  /**
   * @brief Stop workspace syncronization.
   */
  void Shutdown();

  sup::sequencer::Workspace* GetWorkspace() const;

  WorkspaceItem* GetWorkspaceItem() const;

private:
  void OnDomainVariableUpdated();
  void OnWorkspaceEventFromGUI(const WorkspaceEvent& event);

  std::unique_ptr<SequencerWorkspaceListener> m_workspace_listener;
  std::unique_ptr<WorkspaceItemController> m_workspace_item_controller;
  sup::sequencer::Workspace* m_workspace{nullptr};
  WorkspaceItem* m_workspace_item{nullptr};
  bool m_started{false};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_SYNCHRONIZER_H_
