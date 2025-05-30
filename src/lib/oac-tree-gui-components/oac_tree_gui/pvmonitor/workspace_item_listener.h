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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_PVMONITOR_WORKSPACE_ITEM_LISTENER_H_
#define OAC_TREE_GUI_PVMONITOR_WORKSPACE_ITEM_LISTENER_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/jobsystem/domain_events.h>

#include <mvvm/signals/event_types.h>

#include <map>
#include <memory>

namespace mvvm
{
class ModelListener;
}

namespace oac_tree_gui
{

class VariableItem;
class WorkspaceItem;

/**
 * @brief The WorkspaceItemListener class propagates changes from WorkspaceItem to the domain
 * workspace.
 *
 * For the moment, any DataChanged events in value leaves of AnyValueItem lead to the regeneration
 * of the whole AnyValue. It is then set to the corresponding variable in the domain.
 *
 * All other model events are ignored. This means, that during mutual workspace listening we do not
 * expect heavy changes in AnyValueItem's layout (i.e. branch insert/removal).
 */
class WorkspaceItemListener
{
public:
  /**
   * @brief Main c-tor.
   */
  WorkspaceItemListener(WorkspaceItem* workspace_item, sup::oac_tree::Workspace* domain_workspace);

  ~WorkspaceItemListener();

  /**
   * @brief Processes events from the domain.
   *
   * This method shall be called when we want to propagate the domain variable change to the GUI
   * models, without notifying the domain back again. This method shall be called from the GUI
   * thread.
   */
  void ProcessEventFromDomain(const VariableUpdatedEvent& event);

private:
  void ValidateWorkspaces();

  void OnDataChangedEvent(const mvvm::DataChangedEvent& event);
  void OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event);

  void ProcessEventToDomain(VariableItem* variable_item);

  WorkspaceItem* m_workspace_item{nullptr};
  workspace_t* m_domain_workspace{nullptr};
  std::unique_ptr<mvvm::ModelListener> m_listener;
  std::vector<bool> m_block_update_to_domain;
  std::map<VariableItem*, std::size_t> m_item_to_index;
  std::vector<VariableItem*> m_index_to_item;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_PVMONITOR_WORKSPACE_ITEM_LISTENER_H_
