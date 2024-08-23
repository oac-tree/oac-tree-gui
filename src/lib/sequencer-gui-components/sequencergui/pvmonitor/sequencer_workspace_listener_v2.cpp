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

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/domain_events.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>

#include <mvvm/utils/threadsafe_queue.h>

#include <sup/sequencer/variable.h>
#include <sup/sequencer/workspace.h>

namespace sequencergui
{

struct SequencerWorkspaceListenerV2::SequencerWorkspaceListenerImpl
{
  using workspace_t = sup::sequencer::Workspace;
  using callback_guard_t = sup::sequencer::ScopeGuard;

  SequencerWorkspaceListenerV2 *m_impl_owner{nullptr};
  workspace_t *m_domain_workspace{nullptr};
  WorkspaceItem *m_workspace_item{nullptr};

  std::vector<VariableItem *> m_index_to_variable_item;
  std::map<std::string, size_t> m_name_to_index;

  callback_guard_t m_guard;
  mvvm::threadsafe_queue<VariableUpdatedEvent> m_workspace_events;

  SequencerWorkspaceListenerImpl(SequencerWorkspaceListenerV2 *impl_owner,
                                 WorkspaceItem *workspace_item,
                                 sup::sequencer::Workspace *domain_workspace)
      : m_impl_owner(impl_owner)
      , m_workspace_item(workspace_item)
      , m_domain_workspace(domain_workspace)
  {
    ValidateWorkspaces();

    auto variables = m_domain_workspace->GetVariables();
    for (size_t index = 0; index < variables.size(); ++index)
    {
      m_name_to_index[variables[index]->GetName()] = index;
    }

    m_index_to_variable_item = m_workspace_item->GetVariables();
  };

  /**
   * @brief Validates if two workspace match.
   */
  void ValidateWorkspaces() const
  {
    if (!m_domain_workspace)
    {
      throw RuntimeException("Not initialised workspace");
    }

    if (!m_workspace_item)
    {
      throw RuntimeException("Not initialised workspace item");
    }

    if (!AreMatchingWorkspaces(*m_workspace_item, *m_domain_workspace))
    {
      throw RuntimeException("Domain and GUI workspace do not match");
    }

    if (m_domain_workspace->IsSuccessfullySetup())
    {
      throw RuntimeException(
          "Workspace setup has been already done. StartListening() method shall be called before.");
    }
  }

  void SubscribeToDomainWorkspace()
  {
    m_guard = m_domain_workspace->GetCallbackGuard(this);

    auto on_variable_updated =
        [this](const std::string &name, const sup::dto::AnyValue &value, bool connected)
    {
      m_workspace_events.push({m_name_to_index[name], value, connected});
      emit m_impl_owner->VariabledUpdated();
    };
    m_domain_workspace->RegisterGenericCallback(on_variable_updated, this);
  }

  ~SequencerWorkspaceListenerImpl()
  {
    m_guard = callback_guard_t{};
    m_domain_workspace = nullptr;
  }
};

SequencerWorkspaceListenerV2::SequencerWorkspaceListenerV2(
    WorkspaceItem *workspace_item, sup::sequencer::Workspace *domain_workspace, QObject *parent)
    : QObject(parent)
    , p_impl(
          std::make_unique<SequencerWorkspaceListenerImpl>(this, workspace_item, domain_workspace))
{
}

SequencerWorkspaceListenerV2::~SequencerWorkspaceListenerV2() = default;

void SequencerWorkspaceListenerV2::StartListening()
{
  if (p_impl->m_domain_workspace->IsSuccessfullySetup())
  {
    throw RuntimeException(
        "Workspace setup has been already done. StartListening() method shall be called before.");
  }

  connect(this, &SequencerWorkspaceListenerV2::VariabledUpdated, this,
          &SequencerWorkspaceListenerV2::OnDomainVariableUpdated, Qt::QueuedConnection);

  p_impl->SubscribeToDomainWorkspace();
}

int SequencerWorkspaceListenerV2::GetEventCount() const
{
  return static_cast<int>(p_impl->m_workspace_events.size());
}

VariableItem *SequencerWorkspaceListenerV2::GetVariableItem(size_t index) const
{
  return index < p_impl->m_index_to_variable_item.size() ? p_impl->m_index_to_variable_item[index]
                                                         : nullptr;
}

VariableUpdatedEvent SequencerWorkspaceListenerV2::PopEvent() const
{
  VariableUpdatedEvent result;
  p_impl->m_workspace_events.try_pop(result);
  return result;
}

void SequencerWorkspaceListenerV2::OnDomainVariableUpdated()
{
  auto event = PopEvent();
  auto item = GetVariableItem(event.index);
  assert(item);

  UpdateVariableFromEvent(event, *item);
}

}  // namespace sequencergui
