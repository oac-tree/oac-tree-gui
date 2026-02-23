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

#include "oac_tree_gui/viewmodel/favorite_instructions_viewmodel.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/project_model.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/viewmodel/instruction_editor_viewmodel.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

namespace
{
const std::int32_t kExpectedColumnCount = 2;  // as defined in LabelDataRowStrategy
}

/**
 * @brief Tests for FavoriteInstructionsViewModel class with complex drag-and-drop scenario.
 */
class FavoriteInstructionsViewModelExtendedTest : public ::testing::Test
{
public:
  FavoriteInstructionsViewModelExtendedTest()
  {
    m_models.CreateEmpty();
    m_models.GetSequencerModel()->GetProcedureContainer()->Clear();  // our untitled procedure
  }

  ApplicationModels m_models;
};

TEST_F(FavoriteInstructionsViewModelExtendedTest, DragFromInstructionEditorViewModel)
{
  // sequencer model contains procedure with one instruction
  auto sequencer_model = m_models.GetSequencerModel();
  auto procedure =
      sequencer_model->InsertItem<ProcedureItem>(sequencer_model->GetProcedureContainer());
  auto wait = sequencer_model->InsertItem<WaitItem>(procedure->GetInstructionContainer());
  wait->SetDisplayName("abc");

  // we are going to drag this instruction from InstructionEditorViewModel
  InstructionEditorViewModel source_view_model(sequencer_model);
  source_view_model.SetRootSessionItem(procedure->GetInstructionContainer());
  const auto wait_index = source_view_model.index(0, 0);
  const std::unique_ptr<QMimeData> mime_data(source_view_model.mimeData({wait_index}));

  // project model contains two instructions in favorite container
  auto project_model = m_models.GetProjectModel();
  auto sequence0 =
      project_model->InsertItem<SequenceItem>(project_model->GetFavoriteInstructionContainer());
  auto sequence1 =
      project_model->InsertItem<SequenceItem>(project_model->GetFavoriteInstructionContainer());
  EXPECT_EQ(project_model->GetFavoriteInstructionContainer()->GetAllItems().size(), 2);

  // we are going to drop it into FavoriteInstructionsViewModel, between sequence0 and sequence1
  FavoriteInstructionsViewModel target_view_model(project_model);
  target_view_model.SetRootSessionItem(project_model->GetFavoriteInstructionContainer());
  const std::int32_t drop_indicator_row = 1;
  const QModelIndex parent_index = QModelIndex();  // invalid

  EXPECT_TRUE(target_view_model.dropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator_row, 0,
                                             parent_index));

  // favorite instruction container should contain 3 items now
  EXPECT_EQ(project_model->GetFavoriteInstructionContainer()->GetInstructionCount(), 3);
  EXPECT_EQ(
      project_model->GetFavoriteInstructionContainer()->GetInstructions().at(1)->GetDisplayName(),
      "abc");

  // original item is untouched
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructionCount(), 1);
  EXPECT_EQ(wait->GetDisplayName(), "abc");

  // identity of the original item is not preserved in the copy
  EXPECT_NE(
      project_model->GetFavoriteInstructionContainer()->GetInstructions().at(1)->GetIdentifier(),
      wait->GetIdentifier());
}

}  // namespace oac_tree_gui::test
