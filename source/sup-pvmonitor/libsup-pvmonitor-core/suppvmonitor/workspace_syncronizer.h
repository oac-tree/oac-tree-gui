/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_WORKSPACE_SYNCRONIZER_H_
#define LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_WORKSPACE_SYNCRONIZER_H_

#include <QObject>
#include <memory>

namespace sup
{
namespace sequencer
{
class Workspace;
}
}  // namespace sup

namespace sequencergui
{
class WorkspaceItem;
class DomainWorkspaceBuilder;
}  // namespace sequencergui

namespace suppvmonitor
{

class MonitorModel;
class SequencerWorkspaceListener;
class WorkspaceItemController;
class WorkspaceEvent;

//! Provides syncronization of WorkspaceItem with sequencer's Workspace.

//! The PVs are represented by VariableItems stored in WorkspaceItem. The synchronizer object
//! creates underlying sequencer Workspace, attaches to it, and then performs mutual updates
//! between sequencer variables and their counterparts VariableItems.

class WorkspaceSyncronizer : public QObject
{
  Q_OBJECT

public:
  WorkspaceSyncronizer(MonitorModel* model, QObject* parent = nullptr);
  ~WorkspaceSyncronizer() override;

  void OnSetupWorkspaceRequest();

  sup::sequencer::Workspace* GetWorkspace() const;

private:
  void OnDomainVariableUpdated();
  void OnWorkspaceEventFromGUI(const WorkspaceEvent& event);
  sequencergui::WorkspaceItem* GetWorkspaceItem();

  std::unique_ptr<SequencerWorkspaceListener> m_workspace_listener;
  std::unique_ptr<sequencergui::DomainWorkspaceBuilder> m_workspace_builder;
  std::unique_ptr<WorkspaceItemController> m_workspace_item_controller;
  std::unique_ptr<sup::sequencer::Workspace> m_workspace;
  MonitorModel* m_model{nullptr};
};

}  // namespace suppvmonitor

#endif  // LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_WORKSPACE_SYNCRONIZER_H_
