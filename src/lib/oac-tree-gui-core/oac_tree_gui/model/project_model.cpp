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

#include "project_model.h"

#include "composer_settings_item.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

namespace oac_tree_gui
{

ProjectModel::ProjectModel(std::shared_ptr<mvvm::ItemPool> pool)
    : mvvm::ApplicationModel("ProjectModel", std::move(pool))
{
  PopulateModel();
}

void ProjectModel::Clear()
{
  mvvm::ApplicationModel::Clear();  // this replaces root item, and clears command stack
  PopulateModel();
}

InstructionContainerItem* ProjectModel::GetFavoriteInstructionContainer() const
{
  return mvvm::utils::GetTopItem<InstructionContainerItem>(*this);
}

void ProjectModel::PopulateModel()
{
  (void)InsertItem<ComposerSettingsItem>();
  auto container = InsertItem<InstructionContainerItem>();
  auto sequence0 = InsertItem<SequenceItem>(container);
  sequence0->SetDisplayName("sequence0");
  auto wait = InsertItem<SequenceItem>(sequence0);
  (void)wait;
  auto sequence1 = InsertItem<SequenceItem>(container);
  sequence1->SetDisplayName("sequence1");

}

}  // namespace oac_tree_gui
