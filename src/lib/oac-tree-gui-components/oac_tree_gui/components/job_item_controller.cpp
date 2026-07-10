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

#include "job_item_controller.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/procedure_item.h>

#include <mvvm/model/item_utils.h>

namespace oac_tree_gui
{

JobItemController::JobItemController(const procedure_changed_t& procedure_changed)
    : m_procedure_changed(procedure_changed)
{
  if (!m_procedure_changed)
  {
    throw RuntimeException("Callback is not initialised");
  }
}

JobItemController::~JobItemController() = default;

void JobItemController::Subscribe()
{
  Listener()->Connect<mvvm::AboutToRemoveItemEvent>(this,
                                                    &JobItemController::OnAboutToRemoveItemEvent);
  Listener()->Connect<mvvm::ItemInsertedEvent>(this, &JobItemController::OnItemInsertedEvent);
  NotifyOnProcedureChange();
}

void JobItemController::Unsubscribe()
{
  if (m_current_procedure_item != nullptr)
  {
    m_current_procedure_item = nullptr;
    m_procedure_changed(m_current_procedure_item);
  }
}

void JobItemController::OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent& event)
{
  auto [parent, tag_index] = event;
  auto item_to_remove = parent->GetItem(tag_index);

  if (m_current_procedure_item == item_to_remove)
  {
    m_current_procedure_item = nullptr;
    m_procedure_changed(m_current_procedure_item);
  }
}

void JobItemController::OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event)
{
  (void)event;
  NotifyOnProcedureChange();
}

void JobItemController::NotifyOnProcedureChange()
{
  auto new_expanded_procedure = GetItemRef().GetExpandedProcedure();
  if (m_current_procedure_item != new_expanded_procedure)
  {
    m_current_procedure_item = new_expanded_procedure;
    m_procedure_changed(new_expanded_procedure);
  }
}

}  // namespace oac_tree_gui
