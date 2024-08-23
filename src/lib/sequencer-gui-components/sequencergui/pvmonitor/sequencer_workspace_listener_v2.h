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

#ifndef SEQUENCERGUI_PVMONITOR_SEQUENCER_WORKSPACE_LISTENER_V2_H_
#define SEQUENCERGUI_PVMONITOR_SEQUENCER_WORKSPACE_LISTENER_V2_H_

#include <QObject>
#include <memory>

namespace sup::sequencer
{
class Workspace;
}

namespace sequencergui
{

class VariableUpdatedEvent;
class WorkspaceItem;
class VariableItem;

/**
 * @brief The SequencerWorkspaceListener class propagates sequencer domain Workspace events to
 * WorkspaceItem.
 *
 * It collects variable updates in an event queue and updates WorkspaceItem via queued connection.
 */
class SequencerWorkspaceListenerV2 : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * Should be used before domain workspace setup.
   */
  explicit SequencerWorkspaceListenerV2(WorkspaceItem* workspace_item,
                                        sup::sequencer::Workspace* domain_workspace,
                                        QObject* parent = nullptr);
  ~SequencerWorkspaceListenerV2() override;

  /**
   * @brief Starts listening for workspace notifications.
   *
   * Should be called before domain workspace setup. Will stop listening on own destruction.
   */
  void StartListening();

  /**
   * @brief Returns number of events in a queue.
   */
  int GetEventCount() const;

  /**
   * @brief Returns item for given index.
   */
  VariableItem* GetVariableItem(size_t index) const;

signals:
  /**
   * @brief Signal that will be triggered on new variable update. Must be connected with the GUI
   * thread via queued connection.
   */
  void VariabledUpdated();

private:
  /**
   * @brief Takes a workspace event from the queue and returns it to the user. Intended for call
   * from the GUI thread.
   */
  VariableUpdatedEvent PopEvent() const;

  /**
   * @brief Propagate domain variable updates to WorkspaceItem.
   *
   * Since this method is connected via a queued connection, there is no issue with thread safety.
   * Event loop will serialize this slot invocation.
   */
  void OnDomainVariableUpdated();

  struct SequencerWorkspaceListenerImpl;
  std::unique_ptr<SequencerWorkspaceListenerImpl> p_impl;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_SEQUENCER_WORKSPACE_LISTENER_V2_H_
