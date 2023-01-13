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

#ifndef LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_SEQUENCER_WORKSPACE_LISTENER_H_
#define LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_SEQUENCER_WORKSPACE_LISTENER_H_

#include <QObject>
#include <memory>

namespace sup::sequencer
{
class Workspace;
}

namespace suppvmonitor
{

class WorkspaceEvent;

//! Listens to sequencer's Workspace for variable updates. Collects updated values in a queue
//! and notifies consumers from the GUI-thread via the queued connection. The idea is to prevent
//! a callback from the Workspace going inside our GUI thread.

class SequencerWorkspaceListener : public QObject
{
  Q_OBJECT

public:
  SequencerWorkspaceListener(QObject* parent = nullptr);
  ~SequencerWorkspaceListener() override;

  void StartListening(sup::sequencer::Workspace* workspace);
  void StopListening();

  int GetEventCount() const;

  WorkspaceEvent PopEvent() const;

signals:
  //! Signal that will be triggered on new variable update. Must be connected with the GUI thread
  //! via queued connection.
  void VariabledUpdated();

private:
  struct SequencerWorkspaceListenerImpl;
  std::unique_ptr<SequencerWorkspaceListenerImpl> p_impl;
};

}  // namespace suppvmonitor

#endif  // LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_SEQUENCER_WORKSPACE_LISTENER_H_
