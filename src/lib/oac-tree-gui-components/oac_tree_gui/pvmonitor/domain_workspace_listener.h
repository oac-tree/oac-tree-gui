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

#ifndef OAC_TREE_GUI_PVMONITOR_DOMAIN_WORKSPACE_LISTENER_H_
#define OAC_TREE_GUI_PVMONITOR_DOMAIN_WORKSPACE_LISTENER_H_

#include <QObject>
#include <memory>

namespace sup::oac_tree
{
class Workspace;
}

namespace oac_tree_gui
{

struct VariableUpdatedEvent;
class WorkspaceItem;
class VariableItem;

/**
 * @brief The DomainWorkspaceListener class propagates sequencer domain Workspace events to
 * user-provided callback.
 *
 * Its main purpose is to disentangle the domain thread from the GUI thread. The
 * DomainWorkspaceListener collects variable updates in an event queue and calls a callback via
 * queued connection, thus making a callback to be executed in a GUI thread.
 *
 * The class will start listening to the domain on the construction, and stop listening when
 * destructed.
 *
 * It is intended to work in pairs with WorkspaceItemListener::ProcessEventFromDomain
 * method. The latter will not propagate GUI model-changed events back into the domain.
 */
class DomainWorkspaceListener : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * Should be used before domain workspace setup. Provided callback will be executed on GUI side.
   *
   * @param domain_workspace oac-tree domain workspace.
   * @param callback A callback to change VariableItem.
   * @param parent_object Qt parent_object.
   */
  DomainWorkspaceListener(sup::oac_tree::Workspace* domain_workspace,
                          const std::function<void(const VariableUpdatedEvent& event)>& callback,
                          QObject* parent_object = nullptr);
  ~DomainWorkspaceListener() override;

  /**
   * @brief Returns number of events in a queue.
   */
  int GetEventCount() const;

signals:
  /**
   * @brief Signal that will be triggered on new variable update. Must be connected with the GUI
   * thread via queued connection.
   */
  void VariabledUpdated();

private:
  /**
   * @brief Starts listening for workspace notifications.
   */
  void StartListening();

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

  struct DomainWorkspaceListenerImpl;
  std::unique_ptr<DomainWorkspaceListenerImpl> p_impl;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_PVMONITOR_DOMAIN_WORKSPACE_LISTENER_H_
