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

#ifndef SUPPVMONITOR_WORKSPACE_ITEM_CONRTOLLER_H_
#define SUPPVMONITOR_WORKSPACE_ITEM_CONRTOLLER_H_

#include <mvvm/signals/event_types.h>
#include <mvvm/signals/signal_slot.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace mvvm
{
class SessionItem;
}

namespace sequencergui
{
class VariableItem;
class WorkspaceItem;
}  // namespace sequencergui

namespace suppvmonitor
{

class WorkspaceEvent;
class MonitorModel;

//! Controls changes in WorkspaceItem and provide callbacks about those that
//! are relevant for the domain workspace.

class WorkspaceItemController
{
public:
  WorkspaceItemController(MonitorModel* model);

  void ProcessEventFromDomain(const WorkspaceEvent& event);

  sequencergui::VariableItem* GeVariableItemForName(const std::string& name);

  sequencergui::WorkspaceItem* GetWorkspaceItem();

  void SetCallback(const std::function<void(const WorkspaceEvent& event)>& callback);

private:
  void OnModelEvent(const mvvm::event_variant_t& event);
  void ProcessEventToDomain(sequencergui::VariableItem* variable_item);

  MonitorModel* m_model{nullptr};
  std::function<void(const WorkspaceEvent& event)> m_report_callback;
  std::unique_ptr<mvvm::Slot> m_slot;
  std::map<std::string, bool> m_block_update_to_domain;
};

}  // namespace suppvmonitor

#endif  // SUPPVMONITOR_WORKSPACE_ITEM_CONRTOLLER_H_s
