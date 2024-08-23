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

#include "sequencer_workspace_listener_v2.h"

#include "workspace_event.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/model/variable_item.h>

#include <mvvm/utils/threadsafe_queue.h>

#include <sup/sequencer/workspace.h>
#include <sup/sequencer/variable.h>

namespace sequencergui
{

struct SequencerWorkspaceListenerV2::SequencerWorkspaceListenerImpl
{
  using workspace_t = sup::sequencer::Workspace;
  using callback_guard_t = sup::sequencer::ScopeGuard;

  SequencerWorkspaceListenerV2 *m_impl_owner{nullptr};
  workspace_t *m_workspace{nullptr};
  WorkspaceItem *m_workspace_item{nullptr};

  callback_guard_t m_guard;
  mvvm::threadsafe_queue<WorkspaceEvent> m_workspace_events;

  SequencerWorkspaceListenerImpl(SequencerWorkspaceListenerV2 *impl_owner,
                                 WorkspaceItem *workspace_item,
                                 sup::sequencer::Workspace *workspace)
      : m_impl_owner(impl_owner), m_workspace_item(workspace_item), m_workspace(workspace)
  {
    ValidateWorkspaces();
  };

  void ValidateWorkspaces()
  {
    if (!m_workspace)
    {
      throw RuntimeException("Not initialised workspace");
    }

    if (!m_workspace_item)
    {
      throw RuntimeException("Not initialised workspace item");
    }

    std::vector<std::string> variable_item_names;
    for (const auto variable_item : m_workspace_item->GetVariables())
    {
      variable_item_names.push_back(variable_item->GetName());
    }
  }

  void StartListening()
  {
    m_guard = m_workspace->GetCallbackGuard(this);

    auto on_variable_updated =
        [this](const std::string &name, const sup::dto::AnyValue &value, bool connected)
    {
      m_workspace_events.push({name, value, connected});
      emit m_impl_owner->VariabledUpdated();
    };
    m_workspace->RegisterGenericCallback(on_variable_updated, this);
  }

  ~SequencerWorkspaceListenerImpl()
  {
    m_guard = callback_guard_t{};
    m_workspace = nullptr;
  }
};

SequencerWorkspaceListenerV2::SequencerWorkspaceListenerV2(WorkspaceItem *workspace_item,
                                                           sup::sequencer::Workspace *workspace,
                                                           QObject *parent)
    : QObject(parent)
    , p_impl(std::make_unique<SequencerWorkspaceListenerImpl>(this, workspace_item, workspace))
{

}

SequencerWorkspaceListenerV2::~SequencerWorkspaceListenerV2() = default;

void SequencerWorkspaceListenerV2::StartListening()
{
  connect(this, &SequencerWorkspaceListenerV2::VariabledUpdated, this,
          &SequencerWorkspaceListenerV2::OnDomainVariableUpdated, Qt::QueuedConnection);

  p_impl->StartListening();
}

int SequencerWorkspaceListenerV2::GetEventCount() const
{
  return static_cast<int>(p_impl->m_workspace_events.size());
}

WorkspaceEvent SequencerWorkspaceListenerV2::PopEvent() const
{
  WorkspaceEvent result;
  p_impl->m_workspace_events.try_pop(result);
  return result;
}

void SequencerWorkspaceListenerV2::OnDomainVariableUpdated() {}

}  // namespace sequencergui
