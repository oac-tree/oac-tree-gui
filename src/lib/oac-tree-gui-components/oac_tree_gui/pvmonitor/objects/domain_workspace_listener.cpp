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

#include "domain_workspace_listener.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/domain_events.h>
#include <oac_tree_gui/pvmonitor/workspace_monitor_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/utils/threadsafe_queue.h>

#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/workspace.h>

namespace oac_tree_gui
{

struct DomainWorkspaceListener::DomainWorkspaceListenerImpl
{
  using workspace_t = sup::oac_tree::Workspace;
  using callback_guard_t = sup::oac_tree::ScopeGuard;

  DomainWorkspaceListener *m_impl_owner{nullptr};
  workspace_t *m_domain_workspace{nullptr};
  const std::function<void(const VariableUpdatedEvent &event)> m_update_item_callback;

  std::map<std::string, size_t> m_name_to_index;

  callback_guard_t m_guard;
  mvvm::threadsafe_queue<VariableUpdatedEvent> m_workspace_events;

  DomainWorkspaceListenerImpl(
      DomainWorkspaceListener *impl_owner, sup::oac_tree::Workspace *domain_workspace,
      const std::function<void(const VariableUpdatedEvent &event)> &callback)
      : m_impl_owner(impl_owner)
      , m_domain_workspace(domain_workspace)
      , m_update_item_callback(callback)
  {
    ValidateParameters();

    auto variables = m_domain_workspace->GetVariables();
    for (std::size_t index = 0; index < variables.size(); ++index)
    {
      m_name_to_index[variables[index]->GetName()] = index;
    }
  };

  /**
   * @brief Validates input parameters.
   */
  void ValidateParameters() const
  {
    if (!m_domain_workspace)
    {
      throw RuntimeException("Not initialised workspace");
    }

    if (!m_update_item_callback)
    {
      throw RuntimeException("Not initialised callback");
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

  ~DomainWorkspaceListenerImpl()
  {
    m_guard = callback_guard_t{};
    m_domain_workspace = nullptr;
  }
};

DomainWorkspaceListener::DomainWorkspaceListener(
    sup::oac_tree::Workspace *domain_workspace,
    const std::function<void(const VariableUpdatedEvent &event)> &callback, QObject *parent_object)
    : QObject(parent_object)
    , p_impl(std::make_unique<DomainWorkspaceListenerImpl>(this, domain_workspace, callback))
{
  StartListening();
}

DomainWorkspaceListener::~DomainWorkspaceListener() = default;

void DomainWorkspaceListener::StartListening()
{
  if (p_impl->m_domain_workspace->IsSuccessfullySetup())
  {
    throw RuntimeException(
        "Workspace setup has been already done. StartListening() method shall be called before.");
  }

  connect(this, &DomainWorkspaceListener::VariabledUpdated, this,
          &DomainWorkspaceListener::OnDomainVariableUpdated, Qt::QueuedConnection);

  p_impl->SubscribeToDomainWorkspace();
}

int DomainWorkspaceListener::GetEventCount() const
{
  return static_cast<int>(p_impl->m_workspace_events.size());
}

VariableUpdatedEvent DomainWorkspaceListener::PopEvent() const
{
  VariableUpdatedEvent result;
  p_impl->m_workspace_events.try_pop(result);
  return result;
}

void DomainWorkspaceListener::OnDomainVariableUpdated()
{
  auto event = PopEvent();
  p_impl->m_update_item_callback(event);
}

}  // namespace oac_tree_gui
