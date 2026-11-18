/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "sub_tree_scope.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/signals/model_listener.h>

namespace oac_tree_gui
{

SubTreeScope::SubTreeScope(QObject* parent_object) : QObject(parent_object) {}

SubTreeScope::~SubTreeScope() = default;

void SubTreeScope::Reset(InstructionContainerItem* container, const InstructionItem* top_level)
{
  m_container = container;
  m_listener.reset();
  m_loose_identifiers.clear();

  if ((m_container != nullptr) && (m_container->GetModel() != nullptr))
  {
    // the scope may change when instructions are inserted, moved or removed
    m_listener = std::make_unique<mvvm::ModelListener>(m_container->GetModel());
    m_listener->Connect<mvvm::ItemInsertedEvent>([this](const auto&)
                                                 { EmitScopeChangedIfNeeded(); });
    m_listener->Connect<mvvm::ItemRemovedEvent>([this](const auto&)
                                                { EmitScopeChangedIfNeeded(); });
  }

  ApplyScope(top_level);
}

void SubTreeScope::Clear()
{
  Reset(nullptr, nullptr);
}

bool SubTreeScope::SetScope(const InstructionItem* top_level)
{
  if (m_container == nullptr)
  {
    return false;
  }

  const auto* current = GetScope();
  const auto current_identifier =
      (current != nullptr) ? current->GetIdentifier() : m_anchor_identifier;
  const auto identifier = (top_level != nullptr) ? top_level->GetIdentifier() : std::string();
  if (identifier == current_identifier)
  {
    return false;
  }

  // record loose instructions of the SubTree we are leaving
  auto& loose = m_loose_identifiers[current_identifier];
  loose.clear();
  for (const auto* instruction : m_container->GetInstructions())
  {
    const auto& visible_identifier = instruction->GetIdentifier();
    if ((visible_identifier != current_identifier)
        && (m_hidden_identifiers.find(visible_identifier) == m_hidden_identifiers.end()))
    {
      (void)loose.insert(visible_identifier);
    }
  }

  ApplyScope(top_level);
  return true;
}

const InstructionItem* SubTreeScope::GetScope() const
{
  if ((m_container == nullptr) || (m_container->GetModel() == nullptr)
      || m_anchor_identifier.empty())
  {
    return nullptr;
  }

  const auto* anchor =
      dynamic_cast<const InstructionItem*>(m_container->GetModel()->FindItem(m_anchor_identifier));
  if (anchor == nullptr)
  {
    return nullptr;
  }

  const auto* top_level = GetTopLevelInstruction(*anchor);
  if ((top_level == nullptr) || (top_level->GetParent() != m_container)
      || (m_hidden_identifiers.find(top_level->GetIdentifier()) != m_hidden_identifiers.end()))
  {
    return nullptr;
  }
  return top_level;
}

const std::set<std::string>& SubTreeScope::GetHiddenIdentifiers() const
{
  return m_hidden_identifiers;
}

void SubTreeScope::ApplyScope(const InstructionItem* top_level)
{
  m_anchor_identifier = (top_level != nullptr) ? top_level->GetIdentifier() : std::string();
  m_hidden_identifiers.clear();

  if (m_container != nullptr)
  {
    const auto& loose = m_loose_identifiers[m_anchor_identifier];
    for (const auto* instruction : m_container->GetInstructions())
    {
      const auto& identifier = instruction->GetIdentifier();
      if ((identifier != m_anchor_identifier) && (loose.find(identifier) == loose.end()))
      {
        (void)m_hidden_identifiers.insert(identifier);
      }
    }
  }

  EmitScopeChangedIfNeeded();
}

void SubTreeScope::EmitScopeChangedIfNeeded()
{
  const auto* scope = GetScope();
  const auto identifier = (scope != nullptr) ? scope->GetIdentifier() : std::string();
  if (identifier != m_last_scope_identifier)
  {
    m_last_scope_identifier = identifier;
    emit scopeChanged();
  }
}

}  // namespace oac_tree_gui
