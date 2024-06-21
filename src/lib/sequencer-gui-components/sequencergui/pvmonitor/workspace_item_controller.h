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

#ifndef SEQUENCERGUI_PVMONITOR_WORKSPACE_ITEM_CONTROLLER_H_
#define SEQUENCERGUI_PVMONITOR_WORKSPACE_ITEM_CONTROLLER_H_

#include <mvvm/signals/event_types.h>
#include <mvvm/signals/model_listener_fwd.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace sequencergui
{

class VariableItem;
class WorkspaceItem;
class WorkspaceEvent;

//! The WorkspaceItemController class listens for changes on the GUI side (WorkspaceItem) and
//! provide notifications that is relevant to domain Workspace using a callback.
//! It also processes WorkspaceEvent and update GUI model accordningly.

class WorkspaceItemController
{
public:
  explicit WorkspaceItemController(WorkspaceItem* item);
  ~WorkspaceItemController();

  void ProcessEventFromDomain(const WorkspaceEvent& event);

  VariableItem* GeVariableItemForName(const std::string& name);

  WorkspaceItem* GetWorkspaceItem();

  void SetCallback(const std::function<void(const WorkspaceEvent& event)>& callback);

private:
  void OnDataChangedEvent(const mvvm::DataChangedEvent& event);
  void OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event);

  void ProcessEventToDomain(VariableItem* variable_item);

  WorkspaceItem* m_workspace_item{nullptr};
  std::function<void(const WorkspaceEvent& event)> m_report_callback;
  std::unique_ptr<mvvm::ModelListener<>> m_listener;
  std::map<std::string, bool> m_block_update_to_domain;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_ITEM_CONTROLLER_H_
