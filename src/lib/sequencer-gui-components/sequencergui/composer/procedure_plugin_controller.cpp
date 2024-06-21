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

#include "procedure_plugin_controller.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/procedure_preamble_items.h>
#include <sequencergui/model/workspace_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>

namespace sequencergui
{

ProcedurePluginController::ProcedurePluginController(mvvm::ISessionModel *model)
    : m_listener(std::make_unique<mvvm::ModelListener<>>(model))
{
  if (model)
  {
    m_listener->Connect<mvvm::ItemInsertedEvent>(this,
                                                 &ProcedurePluginController::OnItemInsertedEvent);
    m_listener->Connect<mvvm::ItemRemovedEvent>(this,
                                                &ProcedurePluginController::OnItemRemovedEvent);
  }
}

ProcedurePluginController::~ProcedurePluginController() = default;

void ProcedurePluginController::UpdateProcedurePreamble(const mvvm::SessionItem *item)
{
  if (auto procedure_item = mvvm::utils::FindItemUp<ProcedureItem>(item); procedure_item)
  {
    UpdatePluginNames(*procedure_item);
  }
}

bool ProcedurePluginController::IsRelevantParent(const mvvm::SessionItem *item)
{
  const bool is_variable_related = mvvm::utils::FindItemUp<WorkspaceItem>(item);
  const bool is_instruction_related = mvvm::utils::FindItemUp<InstructionContainerItem>(item);

  return is_instruction_related || is_variable_related;
}

void ProcedurePluginController::OnItemInsertedEvent(const mvvm::ItemInsertedEvent &event)
{
  if (IsRelevantParent(event.m_item))
  {
    UpdateProcedurePreamble(event.m_item);
  }
}

void ProcedurePluginController::OnItemRemovedEvent(const mvvm::ItemRemovedEvent &event)
{
  if (IsRelevantParent(event.m_item))
  {
    UpdateProcedurePreamble(event.m_item);
  }
}

}  // namespace sequencergui
