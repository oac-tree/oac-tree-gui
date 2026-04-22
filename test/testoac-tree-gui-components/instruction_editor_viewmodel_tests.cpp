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

#include "oac_tree_gui/viewmodel/instruction_editor_viewmodel.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/composer/instruction_copy_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/transform/transform_from_domain.h>

#include <sup/gui/components/mime_conversion_helper.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/providers/viewitem.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QMimeData>
#include <QSignalSpy>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for InstructionEditorViewModel class.
 */
class InstructionEditorViewModelTest : public ::testing::Test
{
public:
  InstructionEditorViewModelTest() : m_view_model(&m_model, {}) {}

  mvvm::ApplicationModel m_model;
  InstructionEditorViewModel m_view_model;
};

TEST_F(InstructionEditorViewModelTest, SingleInstruction)
{
  auto sequence = m_model.InsertItem<SequenceItem>();
  sequence->SetStatus(InstructionStatus::kRunning);

  EXPECT_EQ(m_view_model.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_view_model.columnCount(QModelIndex()), 2);

  auto sequence_displayname_index = m_view_model.index(0, 0, QModelIndex());
  auto sequence_type_index = m_view_model.index(0, 1, QModelIndex());

  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(sequence_displayname_index), sequence);
  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(sequence_type_index), sequence);

  EXPECT_EQ(m_view_model.data(sequence_displayname_index, Qt::DisplayRole).toString(), "Sequence");
  EXPECT_EQ(m_view_model.data(sequence_type_index, Qt::DisplayRole).toString(), "Sequence");
}

TEST_F(InstructionEditorViewModelTest, SingleUniversalInstruction)
{
  auto fallback = std::make_unique<UniversalInstructionItem>();
  fallback->SetDomainType(domainconstants::kFallbackInstructionType);
  m_model.InsertItem(std::move(fallback), m_model.GetRootItem(), mvvm::TagIndex::Append());

  auto displayname_index = m_view_model.index(0, 0, QModelIndex());
  auto type_index = m_view_model.index(0, 1, QModelIndex());

  EXPECT_EQ(m_view_model.data(displayname_index, Qt::DisplayRole).toString(), "Fallback");
  EXPECT_EQ(m_view_model.data(type_index, Qt::DisplayRole).toString(), "Fallback");
}

TEST_F(InstructionEditorViewModelTest, SequenceWithChild)
{
  auto sequence = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);

  auto sequence_index = m_view_model.index(0, 0, QModelIndex());
  EXPECT_EQ(m_view_model.rowCount(sequence_index), 2);
  EXPECT_EQ(m_view_model.columnCount(sequence_index), 2);

  auto wait0_displayname_index = m_view_model.index(0, 0, sequence_index);
  auto wait1_displayname_index = m_view_model.index(1, 0, sequence_index);

  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(wait0_displayname_index), wait0);
  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(wait1_displayname_index), wait1);

  EXPECT_EQ(m_view_model.data(wait0_displayname_index, Qt::DisplayRole).toString(), "Wait");
  EXPECT_EQ(m_view_model.data(wait1_displayname_index, Qt::DisplayRole).toString(), "Wait");
}

TEST_F(InstructionEditorViewModelTest, NotificationOnNameChange)
{
  mvvm::ApplicationModel model;

  auto sequence = model.InsertItem<SequenceItem>();

  InstructionEditorViewModel view_model(&model);

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);
  EXPECT_EQ(view_model.columnCount(QModelIndex()), 2);

  const QModelIndex label_index = view_model.index(0, 0, QModelIndex());
  const QModelIndex data_index = view_model.index(0, 1, QModelIndex());

  QSignalSpy spy_data_changed(&view_model, &InstructionEditorViewModel::dataChanged);

  sequence->SetDisplayName("abc");
  // we have notifications from displayName
  EXPECT_EQ(spy_data_changed.count(), 1);

  const QList<QVariant> arguments = spy_data_changed.takeLast();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), label_index);
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), label_index);
  const QVector<int> expected_roles = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expected_roles);
}

TEST_F(InstructionEditorViewModelTest, FlagsForDragAndDrop)
{
  auto wait0 = m_model.InsertItem<SequenceItem>();

  // Valid index should have drag and drop enabled
  const Qt::ItemFlags valid_flags = m_view_model.flags(m_view_model.index(0, 0, QModelIndex()));
  EXPECT_TRUE(valid_flags & Qt::ItemIsEnabled);
  EXPECT_TRUE(valid_flags & Qt::ItemIsSelectable);
  EXPECT_TRUE(valid_flags & Qt::ItemIsDragEnabled);
  EXPECT_TRUE(valid_flags & Qt::ItemIsDropEnabled);

  // Invalid index (root) should only have drop enabled
  const Qt::ItemFlags invalid_flags = m_view_model.flags(QModelIndex());
  EXPECT_TRUE(invalid_flags & Qt::ItemIsDropEnabled);
  EXPECT_FALSE(invalid_flags & Qt::ItemIsDragEnabled);
}

TEST_F(InstructionEditorViewModelTest, SupportedActions)
{
  EXPECT_TRUE(m_view_model.supportedDragActions() & Qt::MoveAction);
  EXPECT_TRUE(m_view_model.supportedDragActions() & Qt::CopyAction);
  EXPECT_TRUE(m_view_model.supportedDropActions() & Qt::MoveAction);
  EXPECT_TRUE(m_view_model.supportedDropActions() & Qt::CopyAction);
}

TEST_F(InstructionEditorViewModelTest, MimeTypes)
{
  const QStringList expected_mime_types = {kNewInstructionMimeType, kInstructionIdentifierMimeType,
                                           kCopyInstructionMimeType};
  EXPECT_EQ(expected_mime_types, m_view_model.mimeTypes());
}

TEST_F(InstructionEditorViewModelTest, MimeDataEncoding)
{
  auto sequence = m_model.InsertItem<SequenceItem>();

  auto display_index = m_view_model.index(0, 0, QModelIndex());
  auto data_index = m_view_model.index(0, 1, QModelIndex());

  std::unique_ptr<QMimeData> mime_data(m_view_model.mimeData({display_index, data_index}));
  EXPECT_NE(mime_data, nullptr);

  // mime data has two objects on board

  // instruction identifier
  EXPECT_TRUE(mime_data->hasFormat(kInstructionIdentifierMimeType));
  auto identifiers = sup::gui::GetStringListFromMime(*mime_data, kInstructionIdentifierMimeType);
  EXPECT_EQ(identifiers.size(), 1);
  EXPECT_EQ(identifiers.at(0), sequence->GetIdentifier());

  // and full copy of instruction
  EXPECT_TRUE(mime_data->hasFormat(kCopyInstructionMimeType));
  auto copied_items = sup::gui::CreateSessionItems(*mime_data, kCopyInstructionMimeType);
  EXPECT_EQ(copied_items.size(), 1);
  EXPECT_EQ(copied_items.at(0)->GetType(), sequence->GetDomainType());
}

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

  auto incl_index_col0 = m_view_model.index(0, 0, QModelIndex());  // name
  auto incl_index_col1 = m_view_model.index(0, 1, QModelIndex());  // custom name
  auto sequence_index = m_view_model.index(1, 0, QModelIndex());
  auto wait0_index = m_view_model.index(0, 0, sequence_index);
  auto wait1_index = m_view_model.index(1, 0, sequence_index);
  auto wait2_index = m_view_model.index(2, 0, QModelIndex());

  {  // only one index is selected is allowed
    const std::unique_ptr<QMimeData> mime_data(m_view_model.mimeData({incl_index_col0}));
    EXPECT_TRUE(
        m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1, QModelIndex()));
  }

  {  // going to move Include instruction
    const std::unique_ptr<QMimeData> mime_data(
        m_view_model.mimeData({incl_index_col0, incl_index_col1}));

    // drop after Wait2
    EXPECT_TRUE(m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 3, 0, QModelIndex()));

    // drop before Wait0
    EXPECT_TRUE(
        m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 0, 0, sequence_index));

    // attempt to drop into Wait0
    EXPECT_FALSE(m_view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 0, 0, wait0_index));
  }
}

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

  auto sequence_index = m_view_model.index(0, 0, QModelIndex());
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

  auto incl_index_col0 = m_view_model.index(0, 0, QModelIndex());  // name
  auto incl_index_col1 = m_view_model.index(0, 1, QModelIndex());  // custom name
  auto sequence_index = m_view_model.index(1, 0, QModelIndex());
  auto wait0_index = m_view_model.index(0, 0, sequence_index);
  auto wait1_index = m_view_model.index(1, 0, sequence_index);

  // going to drag Include instruction
  const std::unique_ptr<QMimeData> mime_data(
      m_view_model.mimeData({incl_index_col0, incl_index_col1}));

  // drop between Wait0 and Wait1
  EXPECT_TRUE(m_view_model.dropMimeData(mime_data.get(), Qt::MoveAction, 1, 0, sequence_index));

  // validating new layout
  EXPECT_EQ(m_model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({sequence, wait2}));
  EXPECT_EQ(sequence->GetInstructions(), std::vector<InstructionItem*>({wait0, incl, wait1}));
}

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

  auto sequence_index = m_view_model.index(0, 0, QModelIndex());
  auto wait0_index_name = m_view_model.index(0, 0, sequence_index);
  auto wait0_index_custom_name = m_view_model.index(0, 1, sequence_index);

  // going to drag Include instruction
  const std::unique_ptr<QMimeData> mime_data(
      m_view_model.mimeData({wait0_index_name, wait0_index_custom_name}));

  // drop after Wait2
  EXPECT_TRUE(m_view_model.dropMimeData(mime_data.get(), Qt::MoveAction, 3, 0, sequence_index));

  // validating new layout
  EXPECT_EQ(sequence->GetInstructions(), std::vector<InstructionItem*>({wait1, wait2, wait0}));
}

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

  auto sequence_index = m_view_model.index(0, 0, QModelIndex());
  auto wait2_index_name = m_view_model.index(2, 0, sequence_index);
  auto wait2_index_custom_name = m_view_model.index(2, 1, sequence_index);

  // going to drag Wait2 instruction
  const std::unique_ptr<QMimeData> mime_data(
      m_view_model.mimeData({wait2_index_name, wait2_index_custom_name}));

  // drop before Wait0
  EXPECT_TRUE(m_view_model.dropMimeData(mime_data.get(), Qt::MoveAction, 0, 0, sequence_index));

  // validating new layout
  EXPECT_EQ(sequence->GetInstructions(), std::vector<InstructionItem*>({wait2, wait0, wait1}));
}

TEST_F(InstructionEditorViewModelTest, DropNewInstructionBetweenChildren)
{
  // Sequence
  //    Wait0
  //    Wait1

  auto container = m_model.InsertItem<InstructionContainerItem>();
  auto sequence = m_model.InsertItem<SequenceItem>(container);
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);

  auto container_index = m_view_model.index(0, 0, QModelIndex());
  auto sequence_index = m_view_model.index(0, 0, container_index);
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

TEST_F(InstructionEditorViewModelTest, DropInstructionCopyBetweenTwoChildren)
{
  // Sequence
  //    Wait0
  //    Wait1

  auto container = m_model.InsertItem<InstructionContainerItem>();
  auto sequence = m_model.InsertItem<SequenceItem>(container);
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);

  auto container_index = m_view_model.index(0, 0, QModelIndex());
  auto sequence_index = m_view_model.index(0, 0, container_index);
  auto wait0_index = m_view_model.index(0, 0, sequence_index);
  auto wait1_index = m_view_model.index(1, 0, sequence_index);

  auto message_instruction = CreateInstructionItem(domainconstants::kMessageInstructionType);
  auto mime_data = CreateInstructionSelectionCopyMimeData({message_instruction.get()});

  // drop between Wait0 and Wait1
  EXPECT_TRUE(m_view_model.dropMimeData(mime_data.get(), Qt::CopyAction, 1, 0, sequence_index));

  // validating new layout
  EXPECT_EQ(sequence->GetInstructions().size(), 3);
  EXPECT_EQ(sequence->GetInstructions().at(1)->GetDomainType(),
            domainconstants::kMessageInstructionType);
}

TEST_F(InstructionEditorViewModelTest, ModelReset)
{
  SequencerModel model;
  auto procedure =
      model.InsertItem<ProcedureItem>(model.GetProcedureContainer(), mvvm::TagIndex::Append());
  auto sequence = model.InsertItem<SequenceItem>(procedure->GetInstructionContainer());

  InstructionEditorViewModel view_model(nullptr, {});
  view_model.SetRootSessionItem(procedure->GetInstructionContainer());

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);
  EXPECT_EQ(view_model.columnCount(QModelIndex()), 2);

  // Mimicking the reset of the document. The viewmodel will be reset to show top level item, no
  // crash occur.
  auto root_item = mvvm::utils::CreateEmptyRootItem();
  auto procedure_container = root_item->InsertItem<mvvm::ContainerItem>(mvvm::TagIndex::Append());
  model.ReplaceRootItem(std::move(root_item));

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);
  EXPECT_EQ(view_model.columnCount(QModelIndex()), 2);
  EXPECT_EQ(view_model.rootItem()->GetRowCount(), 1);
  EXPECT_EQ(view_model.rootItem()->GetColumnCount(), 2);
}

}  // namespace oac_tree_gui::test
