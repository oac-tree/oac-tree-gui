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

  workspace_t *m_workspace{nullptr};
  callback_guard_t m_guard;
  SequencerWorkspaceListener *m_self{nullptr};
  mvvm::threadsafe_queue<WorkspaceEvent> m_workspace_events;

  void AttachToWorkspace(workspace_t *workspace)
  {
    if (m_workspace)
    {
      throw RuntimeException("Already existing workspace");
    }

    if (!workspace)
    {
      throw RuntimeException("Not initialised workspace");
    }

    m_workspace = workspace;
    m_guard = m_workspace->GetCallbackGuard(this);

    auto on_variable_updated =
        [this](const std::string &name, const sup::dto::AnyValue &value, bool connected)
    {
      m_workspace_events.push({name, value, connected});
      emit m_self->VariabledUpdated();
    };
    m_workspace->RegisterGenericCallback(on_variable_updated, this);
  }

  void StopListening()
  {
    if (!m_workspace)
    {
      throw RuntimeException("Nothing to listen");
    }

    m_guard = callback_guard_t{};
    m_workspace = nullptr;
  }

  explicit SequencerWorkspaceListenerImpl(SequencerWorkspaceListener *self) : m_self(self){};
};

SequencerWorkspaceListener::SequencerWorkspaceListener(QObject *parent)
    : QObject(parent), p_impl(std::make_unique<SequencerWorkspaceListenerImpl>(this))
{
}

SequencerWorkspaceListener::~SequencerWorkspaceListener() = default;

void SequencerWorkspaceListener::StartListening(sup::sequencer::Workspace *workspace)
{
  p_impl->AttachToWorkspace(workspace);
}

void SequencerWorkspaceListener::StopListening()
{
  p_impl->StopListening();
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
