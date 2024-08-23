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
 * @brief The SequencerWorkspaceListener class listens to the sequencer's Workspace for variable
 * updates.
 *
 * Collects variable updates in an event queue and notifies consumers from the GUI thread via the
 * queued connection. Consumers then can take events one by one using the thread-safe PopEvent
 * method.
 */
class SequencerWorkspaceListener : public QObject
{
  Q_OBJECT

public:
  explicit SequencerWorkspaceListener(sup::sequencer::Workspace* workspace,
                                      QObject* parent = nullptr);
  ~SequencerWorkspaceListener() override;

  /**
   * @brief Starts listening for workspace notifications.
   *
   * This call should be done after connection to VariableUpdated signal, otherwise, the queue might
   * start receiving events before anyone is ready to read it.
   *
   * It will stop listening on own destruction.
   */
  void StartListening();

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
