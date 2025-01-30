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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencer_model.h"

#include <oac-tree-gui/model/procedure_item.h>
#include <oac-tree-gui/model/sequencer_item_helper.h>

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

mvvm::ContainerItem *SequencerModel::GetProcedureContainer() const
{
  return mvvm::utils::GetTopItem<mvvm::ContainerItem>(this);
}

std::vector<ProcedureItem *> SequencerModel::GetProcedures() const
{
  return GetProcedureContainer()->GetItems<ProcedureItem>(mvvm::TagIndex::kDefaultTag);
}

void SequencerModel::CreateUntitledProcedure()
{
  auto procedure = InsertItem<ProcedureItem>(GetProcedureContainer());
  procedure->SetDisplayName("Untitled");
}

void SequencerModel::Clear()
{
  mvvm::ApplicationModel::Clear();  // this replaces root item, and clears command stack
  PopulateModel();
}

void SequencerModel::PopulateModel()
{
  InsertItem<mvvm::ContainerItem>()->SetDisplayName("Procedure container");
  if (auto command_stack = GetCommandStack(); command_stack)
  {
    command_stack->Clear();  // to disallow "undo" of our main container
  }
}

// ------------------------------------------------------------------------------------------------
// SequencerProjectModel
// ------------------------------------------------------------------------------------------------

SequencerProjectModel::SequencerProjectModel(std::shared_ptr<mvvm::ItemPool> pool)
    : SequencerModel(std::move(pool))
{
}

void SequencerProjectModel::Clear()
{
  SequencerModel::Clear();
  CreateUntitledProcedure();
}

}  // namespace oac_tree_gui
