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

#ifndef SEQUENCERGUI_PVMONITOR_SEQUENCER_WORKSPACE_LISTENER_H_
#define SEQUENCERGUI_PVMONITOR_SEQUENCER_WORKSPACE_LISTENER_H_

#include <QObject>
#include <memory>

namespace sup::sequencer
{
class Workspace;
}

namespace sequencergui
{

class WorkspaceEvent;

/**
 * @brief The SequencerWorkspaceListener class listens to sequencer's Workspace for variable
 * updates.
 *
 * Collects updated values in a queue and notifies consumers from the GUI thread via the queued
 * connection. The idea is to prevent a callback from the Workspace from going inside our GUI
 * thread.
 */
class SequencerWorkspaceListener : public QObject
{
  Q_OBJECT

public:
  explicit SequencerWorkspaceListener(QObject* parent = nullptr);
  ~SequencerWorkspaceListener() override;

  /**
   * @brief Starts listening for workspace notifications.
   */
  void StartListening(sup::sequencer::Workspace* workspace);

  /**
   * @brief Stops listening for workspace notifications.
   */
  void StopListening();

  /**
   * @brief Returns number of events in a queue.
   */
  int GetEventCount() const;

  /**
   * @brief Takes a workspace event from the queue and returns it to the user. Intended for call
   * from the GUI thread.
   */
  WorkspaceEvent PopEvent() const;

signals:
  /**
   * @brief Signal that will be triggered on new variable update. Must be connected with the GUI
   * thread via queued connection.
   */
  void VariabledUpdated();

private:
  struct SequencerWorkspaceListenerImpl;
  std::unique_ptr<SequencerWorkspaceListenerImpl> p_impl;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_SEQUENCER_WORKSPACE_LISTENER_H_
