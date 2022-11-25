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

#include <sequencergui/core/exceptions.h>
#include <sup/sequencer/workspace.h>
#include <suppvmonitor/sequencer_workspace_listener.h>

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

  void AttachToWorkspace(workspace_t *workspace)
  {
    if (m_workspace)
    {
      throw sequencergui::RuntimeException("Already existing workspace");
    }

    if (!workspace)
    {
      throw sequencergui::RuntimeException("Non initialised workspace");
    }

    m_workspace = workspace;
    m_guard = m_workspace->GetCallbackGuard(this);
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

  SequencerWorkspaceListenerImpl(SequencerWorkspaceListener *self) : m_self(self){};
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

}  // namespace suppvmonitor
