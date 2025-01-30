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

#include "oac-tree-gui/viewmodel/instruction_editor_viewmodel.h"

#include <oac-tree-gui/model/instruction_container_item.h>
#include <oac-tree-gui/model/procedure_item.h>
#include <oac-tree-gui/model/sequencer_item_helper.h>
#include <oac-tree-gui/model/sequencer_model.h>
#include <oac-tree-gui/model/standard_instruction_items.h>
#include <oac-tree-gui/viewmodel/drag_and_drop_helper.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <gtest/gtest.h>

#include <QMimeData>
#include <QSignalSpy>

namespace oac_tree_gui::test
{

//! Tests for InstructionEditorViewModel class.

class InstructionEditorViewModelTest : public ::testing::Test
{
public:
  InstructionEditorViewModelTest() : m_view_model(&m_model) {}

  mvvm::ApplicationModel m_model;
  InstructionEditorViewModel m_view_model;
};

//! Single instruction in a model. ViewModel should see single row and 2 columns.

TEST_F(InstructionEditorViewModelTest, SingleInstruction)
{
  auto sequence = m_model.InsertItem<SequenceItem>();
  sequence->SetStatus("abc");

  EXPECT_EQ(m_view_model.rowCount(), 1);
  EXPECT_EQ(m_view_model.columnCount(), 2);

  auto sequence_displayname_index = m_view_model.index(0, 0);
  auto sequence_customname_index = m_view_model.index(0, 1);

  auto views = m_view_model.FindViews(GetNameItem(*sequence));
  EXPECT_EQ(views.size(), 1);
  EXPECT_EQ(m_view_model.indexFromItem(views[0]), sequence_customname_index);

  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(sequence_displayname_index), sequence);
  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(sequence_customname_index),
            GetNameItem(*sequence));

  EXPECT_EQ(m_view_model.data(sequence_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Sequence"));
  EXPECT_EQ(m_view_model.data(sequence_customname_index, Qt::DisplayRole).toString().toStdString(),
            std::string(""));
}

//! Sequence with a child in a model.

TEST_F(InstructionEditorViewModelTest, SequenceWithChild)
{
  auto sequence = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);

  auto sequence_index = m_view_model.index(0, 0);
  EXPECT_EQ(m_view_model.rowCount(sequence_index), 2);
  EXPECT_EQ(m_view_model.columnCount(sequence_index), 2);

  auto wait0_displayname_index = m_view_model.index(0, 0, sequence_index);
  auto wait1_displayname_index = m_view_model.index(1, 0, sequence_index);

  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(wait0_displayname_index), wait0);
  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(wait1_displayname_index), wait1);

  EXPECT_EQ(m_view_model.data(wait0_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait"));
  EXPECT_EQ(m_view_model.data(wait1_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait"));
}

//! Check that view model send notification on name change.

TEST_F(InstructionEditorViewModelTest, NotificationOnDataChange)
{
  auto sequence = m_model.InsertItem<SequenceItem>();

  EXPECT_EQ(m_view_model.rowCount(), 1);
  EXPECT_EQ(m_view_model.columnCount(), 2);

  QSignalSpy spy_data_changed(&m_view_model, &InstructionEditorViewModel::dataChanged);

  sequence->SetName("abc");
  EXPECT_EQ(spy_data_changed.count(), 1);
}

//! Validating method CanDropMimeData for move operation.

TEST_F(InstructionEditorViewModelTest, CanDropMoveMimeData)
{
  // Include
  // Sequence
  //    Wait0
  //    Wait1
  // Wait2

  auto incl = m_model.InsertItem<IncludeItem>();
  auto sequence = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>();

  auto incl_index_col0 = m_view_model.index(0, 0);  // name
  auto incl_index_col1 = m_view_model.index(0, 1);  // custom name
  auto sequence_index = m_view_model.index(1, 0);
  auto wait0_index = m_view_model.index(0, 0, sequence_index);
  auto wait1_index = m_view_model.index(1, 0, sequence_index);
  auto wait2_index = m_view_model.index(2, 0);

  // we can't perform drop if only one cell is selected
  auto mime_data = CreateInstructionMoveMimeData({incl_index_col0});
  EXPECT_FALSE(
      m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1, QModelIndex()));

  // going to move Include instruction
  mime_data = CreateInstructionMoveMimeData({incl_index_col0, incl_index_col1});

  // drop after Wait2
  EXPECT_TRUE(m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 3, 0, QModelIndex()));

  // drop before Wait0
  EXPECT_TRUE(m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 0, 0, sequence_index));

  // attempt to drop into Wait0
  EXPECT_FALSE(m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 0, 0, wait0_index));
}

//! Validating method CanDropMimeData for insert new operation.

TEST_F(InstructionEditorViewModelTest, CanDropNewMimeData)
{
  // Sequence
  //    Wait0
  //    Wait1
  // Wait2

  auto sequence = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>();

  auto sequence_index = m_view_model.index(0, 0);
  auto wait0_index = m_view_model.index(0, 0, sequence_index);
  auto wait1_index = m_view_model.index(1, 0, sequence_index);

  // we can't perform drop if only one cell is selected
  auto mime_data = CreateNewInstructionMimeData(
      QString::fromStdString(domainconstants::kIncludeInstructionType));

  // dropping before sequence
  EXPECT_TRUE(m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 0, 0, QModelIndex()));

  // dropping between two waits
  EXPECT_TRUE(m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 1, 0, sequence_index));

  // dropping onto wait1
  EXPECT_FALSE(m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1, wait1_index));
}

//! Validating method dropMimeData.
//! Item is moved from one parent to another and inserted between its children

TEST_F(InstructionEditorViewModelTest, DropMimeDataBetweenFromDifferentParent)
{
  // Include
  // Sequence
  //    Wait0
  //    Wait1
  // Wait2

  auto incl = m_model.InsertItem<IncludeItem>();
  auto sequence = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>();

  auto incl_index_col0 = m_view_model.index(0, 0);  // name
  auto incl_index_col1 = m_view_model.index(0, 1);  // custom name
  auto sequence_index = m_view_model.index(1, 0);
  auto wait0_index = m_view_model.index(0, 0, sequence_index);
  auto wait1_index = m_view_model.index(1, 0, sequence_index);

  // going to drag Include instruction
  auto mime_data = CreateInstructionMoveMimeData({incl_index_col0, incl_index_col1});

  // drop between Wait0 and Wait1
  EXPECT_TRUE(m_view_model.dropMimeData(mime_data.get(), Qt::MoveAction, 1, 0, sequence_index));

  // validating new layout
  EXPECT_EQ(m_model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({sequence, wait2}));
  EXPECT_EQ(sequence->GetInstructions(), std::vector<InstructionItem*>({wait0, incl, wait1}));
}

//! Validating method dropMimeData.
//! Item is moved within athe same parent from first position to the last.

TEST_F(InstructionEditorViewModelTest, DropMimeDataFirstToLast)
{
  // Sequence
  //    Wait0
  //    Wait1
  //    Wait2

  auto sequence = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  auto sequence_index = m_view_model.index(0, 0);
  auto wait0_index_name = m_view_model.index(0, 0, sequence_index);
  auto wait0_index_custom_name = m_view_model.index(0, 0, sequence_index);

  // going to drag Include instruction
  auto mime_data = CreateInstructionMoveMimeData({wait0_index_name, wait0_index_custom_name});

  // drop after Wait2
  EXPECT_TRUE(m_view_model.dropMimeData(mime_data.get(), Qt::MoveAction, 3, 0, sequence_index));

  // validating new layout
  EXPECT_EQ(sequence->GetInstructions(), std::vector<InstructionItem*>({wait1, wait2, wait0}));
}

//! Validating method dropMimeData.
//! Item is moved within athe same parent from first position to the last.

TEST_F(InstructionEditorViewModelTest, DropMimeDataLastToFirst)
{
  // Sequence
  //    Wait0
  //    Wait1
  //    Wait2

  auto sequence = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  auto sequence_index = m_view_model.index(0, 0);
  auto wait2_index_name = m_view_model.index(2, 0, sequence_index);
  auto wait2_index_custom_name = m_view_model.index(2, 0, sequence_index);

  // going to drag Include instruction
  auto mime_data = CreateInstructionMoveMimeData({wait2_index_name, wait2_index_custom_name});

  // drop before Wait0
  EXPECT_TRUE(m_view_model.dropMimeData(mime_data.get(), Qt::MoveAction, 0, 0, sequence_index));

  // validating new layout
  EXPECT_EQ(sequence->GetInstructions(), std::vector<InstructionItem*>({wait2, wait0, wait1}));
}

//! New item is created by dropping between two children.

TEST_F(InstructionEditorViewModelTest, DropNewInstructionBetweenChildren)
{
  // Sequence
  //    Wait0
  //    Wait1

  auto sequence = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);

  auto sequence_index = m_view_model.index(0, 0);
  auto wait0_index = m_view_model.index(0, 0, sequence_index);
  auto wait1_index = m_view_model.index(1, 0, sequence_index);

  // going to drag Include instruction
  auto mime_data = CreateNewInstructionMimeData(
      QString::fromStdString(domainconstants::kIncludeInstructionType));

  // drop between Wait0 and Wait1
  EXPECT_TRUE(m_view_model.dropMimeData(mime_data.get(), Qt::MoveAction, 1, 0, sequence_index));

  // validating new layout
  EXPECT_EQ(sequence->GetInstructions().size(), 3);
  EXPECT_EQ(sequence->GetInstructions().at(1)->GetDomainType(),
            domainconstants::kIncludeInstructionType);
}

//! Single instruction in a model. ViewModel should see single row and 2 columns.

TEST_F(InstructionEditorViewModelTest, ModelReset)
{
  SequencerModel model;
  auto procedure =
      model.InsertItem<ProcedureItem>(model.GetProcedureContainer(), mvvm::TagIndex::Append());
  auto sequence = model.InsertItem<SequenceItem>(procedure->GetInstructionContainer());

  InstructionEditorViewModel view_model(nullptr);
  view_model.SetRootSessionItem(procedure->GetInstructionContainer());

  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 2);

  // Mimicking load of the procedure from disk. The procedure doesn't contain instruction container
  // and is not suitable for displaying by InstructionEditorViewModel.
  auto root_item = mvvm::utils::CreateEmptyRootItem();
  auto procedure_container = root_item->InsertItem<mvvm::ContainerItem>(mvvm::TagIndex::Append());
  model.ReplaceRootItem(std::move(root_item));

  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 2);

  EXPECT_EQ(view_model.rootItem()->GetRowCount(), 1);
  EXPECT_EQ(view_model.rootItem()->GetColumnCount(), 2);  // Failing here with, gives 1
}

}  // namespace oac_tree_gui::test
