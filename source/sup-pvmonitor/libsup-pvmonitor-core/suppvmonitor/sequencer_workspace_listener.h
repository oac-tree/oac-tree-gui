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

#ifndef SUPPVMONITOR_SEQUENCER_WORKSPACE_LISTENER_H_
#define SUPPVMONITOR_SEQUENCER_WORKSPACE_LISTENER_H_

#include <QObject>
#include <memory>

namespace sup::sequencer
{
class Workspace;
}

namespace suppvmonitor
{

//! Listens sequencer's Workspace for update in variables. Collect updated values in a queue and
//! notifies consumers from the GUI-thread via queued connection.

class SequencerWorkspaceListener : public QObject
{
  Q_OBJECT

public:
  SequencerWorkspaceListener(QObject* parent = nullptr);
  ~SequencerWorkspaceListener() override;

  void StartListening(sup::sequencer::Workspace* workspace);
  void StopListening();

signals:
  void VariabledUpdated();

private:
  struct SequencerWorkspaceListenerImpl;
  std::unique_ptr<SequencerWorkspaceListenerImpl> p_impl;
};

}  // namespace suppvmonitor

#endif  // SUPPVMONITOR_SEQUENCER_WORKSPACE_LISTENER_H_
