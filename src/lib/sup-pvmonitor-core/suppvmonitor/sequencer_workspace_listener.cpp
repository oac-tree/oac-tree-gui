/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <sequencergui/core/exceptions.h>
#include <suppvmonitor/sequencer_workspace_listener.h>
#include <suppvmonitor/workspace_event.h>

#include <mvvm/utils/threadsafe_queue.h>

#include <sup/sequencer/workspace.h>

namespace suppvmonitor
{

struct SequencerWorkspaceListener::SequencerWorkspaceListenerImpl
{
  using workspace_t = sup::sequencer::Workspace;
  using manager_t = sup::sequencer::NamedCallbackManager<const sup::dto::AnyValue &>;
  using callback_guard_t = sup::sequencer::CallbackGuard<manager_t>;

  workspace_t *m_workspace{nullptr};
  callback_guard_t m_guard{nullptr, nullptr};
  SequencerWorkspaceListener *m_self{nullptr};
  mvvm::threadsafe_queue<WorkspaceEvent> m_workspace_events;

  void AttachToWorkspace(workspace_t *workspace)
  {
    if (m_workspace)
    {
      throw sequencergui::RuntimeException("Already existing workspace");
    }

    if (!workspace)
    {
      throw sequencergui::RuntimeException("Not initialised workspace");
    }

    m_workspace = workspace;
    m_guard = m_workspace->GetCallbackGuard(this);

    auto on_variable_updated = [this](const std::string &name, const sup::dto::AnyValue &value)
    {
      m_workspace_events.push({name, value});
      emit m_self->VariabledUpdated();
    };
    m_workspace->RegisterGenericCallback(on_variable_updated, this);
  }

  void StopListening()
  {
    if (!m_workspace)
    {
      throw sequencergui::RuntimeException("Nothing to listen");
    }

    m_guard = callback_guard_t{nullptr, nullptr};
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
  return p_impl->m_workspace_events.size();
}

//! Takes a workspace event from the queue and returns it to the user.
//! Intended for call from the GUI thread.

WorkspaceEvent SequencerWorkspaceListener::PopEvent() const
{
  WorkspaceEvent result;
  p_impl->m_workspace_events.try_pop(result);
  return result;
}

}  // namespace suppvmonitor
