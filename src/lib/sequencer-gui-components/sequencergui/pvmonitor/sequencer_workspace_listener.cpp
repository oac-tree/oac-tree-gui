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

#include "sequencer_workspace_listener.h"

#include "workspace_event.h"

#include <sequencergui/core/exceptions.h>

#include <mvvm/utils/threadsafe_queue.h>

#include <sup/sequencer/workspace.h>

namespace sequencergui
{

struct SequencerWorkspaceListener::SequencerWorkspaceListenerImpl
{
  using workspace_t = sup::sequencer::Workspace;
  using callback_guard_t = sup::sequencer::ScopeGuard;

  SequencerWorkspaceListener *m_impl_owner{nullptr};
  workspace_t *m_workspace{nullptr};
  callback_guard_t m_guard;
  mvvm::threadsafe_queue<WorkspaceEvent> m_workspace_events;

  SequencerWorkspaceListenerImpl(SequencerWorkspaceListener *impl_owner,
                                 sup::sequencer::Workspace *workspace)
      : m_impl_owner(impl_owner), m_workspace(workspace)
  {
    if (!m_workspace)
    {
      throw RuntimeException("Not initialised workspace");
    }
  };

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

SequencerWorkspaceListener::SequencerWorkspaceListener(sup::sequencer::Workspace *workspace,
                                                       QObject *parent)
    : QObject(parent), p_impl(std::make_unique<SequencerWorkspaceListenerImpl>(this, workspace))
{
}

SequencerWorkspaceListener::~SequencerWorkspaceListener() = default;

void SequencerWorkspaceListener::StartListening()
{
  p_impl->StartListening();
}

int SequencerWorkspaceListener::GetEventCount() const
{
  return static_cast<int>(p_impl->m_workspace_events.size());
}

WorkspaceEvent SequencerWorkspaceListener::PopEvent() const
{
  WorkspaceEvent result;
  p_impl->m_workspace_events.try_pop(result);
  return result;
}

}  // namespace sequencergui
