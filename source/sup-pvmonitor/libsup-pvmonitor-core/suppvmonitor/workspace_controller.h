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

#ifndef SUPPVMONITOR_WORKSPACE_CONTROLLER_H_
#define SUPPVMONITOR_WORKSPACE_CONTROLLER_H_

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
}

namespace suppvmonitor
{

class MonitorModel;
class SequencerWorkspaceListener;

//!

class WorkspaceController : public QObject
{
  Q_OBJECT

public:
  WorkspaceController(MonitorModel* model, QObject* parent = nullptr);
  ~WorkspaceController() override;

  void OnSetupWorkspaceRequest();

  void ProcessVariable();

  sup::sequencer::Workspace* GetWorkspace() const;

private:
  sequencergui::WorkspaceItem* GetWorkspaceItem();

  std::unique_ptr<SequencerWorkspaceListener> m_workspace_listener;
  std::unique_ptr<sequencergui::DomainWorkspaceBuilder> m_workspace_builder;
  std::unique_ptr<sup::sequencer::Workspace> m_workspace;
  MonitorModel* m_model{nullptr};
};

}  // namespace suppvmonitor

#endif  // SUPPVMONITOR_WORKSPACE_CONTROLLER_H_
