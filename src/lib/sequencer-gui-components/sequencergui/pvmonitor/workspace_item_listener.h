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

#ifndef SEQUENCERGUI_PVMONITOR_WORKSPACE_ITEM_LISTENER_H_
#define SEQUENCERGUI_PVMONITOR_WORKSPACE_ITEM_LISTENER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <mvvm/signals/event_types.h>
#include <mvvm/signals/model_listener_fwd.h>

#include <memory>

namespace sequencergui
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
  WorkspaceItemListener(WorkspaceItem* workspace_item, sup::sequencer::Workspace* domain_workspace);

  ~WorkspaceItemListener();

private:
  void OnDataChangedEvent(const mvvm::DataChangedEvent& event);

  void ProcessEventToDomain(VariableItem* variable_item);

  WorkspaceItem* m_workspace_item{nullptr};
  workspace_t* m_domain_workspace{nullptr};
  std::unique_ptr<mvvm::ModelListener<>> m_listener;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_ITEM_LISTENER_H_
