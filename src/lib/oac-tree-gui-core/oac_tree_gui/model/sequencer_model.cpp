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

#include "sequencer_model.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

namespace oac_tree_gui
{

SequencerModel::SequencerModel(std::shared_ptr<mvvm::ItemPool> pool)
    : mvvm::ApplicationModel("SequencerModel", std::move(pool))

{
  PopulateModel();
}

mvvm::ContainerItem* SequencerModel::GetProcedureContainer() const
{
  return mvvm::utils::GetTopItem<mvvm::ContainerItem>(*this);
}

std::vector<ProcedureItem*> SequencerModel::GetProcedures() const
{
  return GetProcedureContainer()->GetItems<ProcedureItem>(mvvm::TagIndex::GetDefaultTag());
}

void SequencerModel::CreateUntitledProcedure()
{
  auto procedure = InsertItem<ProcedureItem>(GetProcedureContainer());
  (void)procedure->SetDisplayName("Untitled");
}

void SequencerModel::Clear()
{
  mvvm::ApplicationModel::Clear();  // this replaces root item, and clears command stack
  PopulateModel();
}

void SequencerModel::PopulateModel()
{
  (void)InsertItem<mvvm::ContainerItem>()->SetDisplayName("Procedure container");
  if (auto command_stack = GetCommandStack(); command_stack)
  {
    command_stack->Clear();  // to disallow "undo" of our main container
  }
}

}  // namespace oac_tree_gui
