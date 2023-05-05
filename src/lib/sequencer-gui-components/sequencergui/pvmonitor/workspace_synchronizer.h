/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

//! Provides data syncronization between WorkspaceItem and sequencer Workspace. All AnyValue
//! updates in domain Workspace will be propagated to WorkspaceItem. Similarly, all DataChangedEvent
//! on GUI model side will be propagated to Sequencer Workspace.
//!
//! @note It is required, that WorkspaceItem and domain Workspace have same amount of variables and
//! their names are the  same.
//!
//! @note It is expected that the method Workspace::Setup() in the domain has not been called yet.
//! See explanations in Start() method's code.

class WorkspaceSynchronizer : public QObject
{
  Q_OBJECT

public:
  WorkspaceSynchronizer(WorkspaceItem* workspace_item, sup::sequencer::Workspace* domain_workspace,
                        QObject* parent = nullptr);
  ~WorkspaceSynchronizer() override;

  void Start();

  sup::sequencer::Workspace* GetWorkspace() const;

  WorkspaceItem* GetWorkspaceItem() const;

private:
  void SetInitialValuesFromDomain();
  void OnDomainVariableUpdated();
  void OnWorkspaceEventFromGUI(const WorkspaceEvent& event);

  std::unique_ptr<SequencerWorkspaceListener> m_workspace_listener;
  std::unique_ptr<WorkspaceItemController> m_workspace_item_controller;

  sup::sequencer::Workspace* m_workspace{nullptr};
  WorkspaceItem* m_workspace_item{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_SYNCHRONIZER_H_
