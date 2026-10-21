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

#include "oac_tree_gui/components/custom_presentation_items.h"

#include <oac_tree_gui/components/component_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QColor>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for presentation items defined in custom_presentation_items.h
 */
class CustomPresentationItemTest : public ::testing::Test
{
public:
  class TestItem : public mvvm::CompoundItem
  {
  public:
    static inline const std::string kIsAvailable = "is_available";

    TestItem() : CompoundItem("TestItem") { AddProperty("is_available", true); }
  };
};

TEST_F(CustomPresentationItemTest, ChannelPresentationItem)
{
  TestItem item;
  const ChannelPresentationItem presentation(item.FindItem(TestItem::kIsAvailable), "channel_name");

  // it should looke like non-editable label with colored rectangle

  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_TRUE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_TRUE(presentation.Data(Qt::DecorationRole).isValid());

  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("channel_name"));
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), GetConnectedVariableColor());

  // changing IsAvailable status should change the color of the rectangle
  EXPECT_TRUE(item.SetProperty(TestItem::kIsAvailable, false));
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), GetDisonnectedVariableColor());

  EXPECT_EQ(presentation.GetQtRolesToEmit(mvvm::DataRole::kData),
            QVector<int>({Qt::DecorationRole}));
}

//! Attribute without text: the value presentation.
TEST_F(CustomPresentationItemTest, AttributePresentationItemValue)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
  const AttributePresentationItem presentation(&item);

  // not set: gray placeholder
  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("default"));
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::gray));
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());

  EXPECT_TRUE(item.SetData(mvvm::uint32{42}));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("42"));

  EXPECT_EQ(presentation.GetQtRolesToEmit(mvvm::DataRole::kData),
            QVector<int>({Qt::DisplayRole, Qt::EditRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(itemconstants::kAttributeTextRole),
            QVector<int>({Qt::DisplayRole, Qt::EditRole, Qt::ForegroundRole, Qt::CheckStateRole}));
}

//! Attribute with text: the cell shows and edits the text, the value stays.
TEST_F(CustomPresentationItemTest, AttributePresentationItemText)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
  EXPECT_TRUE(item.SetData(mvvm::uint32{42}));
  AttributePresentationItem presentation(&item);

  EXPECT_TRUE(item.SetText("$par"));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("$par"));
  EXPECT_EQ(presentation.Data(Qt::EditRole).toString(), QString("$par"));
  EXPECT_TRUE(presentation.IsEditable());

  EXPECT_TRUE(presentation.SetData(QString("$abc"), Qt::EditRole));
  EXPECT_EQ(item.GetText(), std::string("$abc"));
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});

  // empty text leaves the text state
  EXPECT_TRUE(presentation.SetData(QString(), Qt::EditRole));
  EXPECT_EQ(item.GetState(), AttributeState::kValue);
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("42"));
}

//! Attribute with text and without value: no gray placeholder.
TEST_F(CustomPresentationItemTest, AttributePresentationItemTextWithoutValue)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
  const AttributePresentationItem presentation(&item);

  EXPECT_TRUE(item.SetText("$par"));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("$par"));
  EXPECT_NE(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::gray));
}

//! Non-nullable bool: the checkbox goes away with text and comes back without it.
TEST_F(CustomPresentationItemTest, AttributePresentationItemNonNullableBool)
{
  AttributeItem item;
  item.SetAttributeType(false, /*is_nullable*/ false, /*allow_text*/ true);
  AttributePresentationItem presentation(&item);

  EXPECT_TRUE(presentation.IsCheckable());
  EXPECT_FALSE(presentation.IsEditable());
  EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

  EXPECT_TRUE(item.SetText("$par"));
  EXPECT_FALSE(presentation.IsCheckable());
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_FALSE(presentation.SetData(Qt::Checked, Qt::CheckStateRole));
  EXPECT_EQ(item.Data(), mvvm::variant_t{false});

  EXPECT_TRUE(presentation.SetData(QString(), Qt::EditRole));
  EXPECT_TRUE(presentation.IsCheckable());
  EXPECT_TRUE(presentation.SetData(Qt::Checked, Qt::CheckStateRole));
  EXPECT_EQ(item.Data(), mvvm::variant_t{true});
}

TEST_F(CustomPresentationItemTest, ExclusiveCheckStatePresentationItem)
{
  InstructionContainerItem container;
  auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
  auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
  auto sequence2 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);

  // all three unchecked
  EXPECT_FALSE(sequence0->IsRoot());
  EXPECT_FALSE(sequence1->IsRoot());
  EXPECT_FALSE(sequence2->IsRoot());

  auto root_property1 = GetIsRootItem(*sequence1);

  ExclusiveCheckStatePresentationItem presentation1(root_property1);

  EXPECT_EQ(presentation1.GetInstructionContainer(), &container);
  EXPECT_EQ(presentation1.GetInstructionItem(), sequence1);
  EXPECT_EQ(presentation1.GetSiblings(), std::vector<InstructionItem*>({sequence0, sequence2}));

  EXPECT_FALSE(presentation1.Data(Qt::DisplayRole).isValid());  // no text
  EXPECT_TRUE(presentation1.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation1.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

  // checking middle item
  EXPECT_TRUE(presentation1.SetData(QVariant(Qt::Checked), Qt::CheckStateRole));

  EXPECT_FALSE(sequence0->IsRoot());
  EXPECT_TRUE(sequence1->IsRoot());
  EXPECT_FALSE(sequence2->IsRoot());

  // checking first item
  auto root_property0 = GetIsRootItem(*sequence0);
  ExclusiveCheckStatePresentationItem presentation0(root_property0);
  EXPECT_TRUE(presentation0.SetData(QVariant(Qt::Checked), Qt::CheckStateRole));

  EXPECT_TRUE(sequence0->IsRoot());
  EXPECT_FALSE(sequence1->IsRoot());
  EXPECT_FALSE(sequence2->IsRoot());

  // checking last item
  auto root_property2 = GetIsRootItem(*sequence2);
  ExclusiveCheckStatePresentationItem presentation2(root_property2);
  EXPECT_TRUE(presentation2.SetData(QVariant(Qt::Checked), Qt::CheckStateRole));

  EXPECT_FALSE(sequence0->IsRoot());
  EXPECT_FALSE(sequence1->IsRoot());
  EXPECT_TRUE(sequence2->IsRoot());

  EXPECT_EQ(presentation0.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
  EXPECT_EQ(presentation1.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Checked);

  // checking same item again
  EXPECT_FALSE(presentation2.SetData(QVariant(Qt::Checked), Qt::CheckStateRole));
  EXPECT_FALSE(sequence0->IsRoot());
  EXPECT_FALSE(sequence1->IsRoot());
  EXPECT_TRUE(sequence2->IsRoot());
}

TEST_F(CustomPresentationItemTest, ExclusiveCheckStateInUndoScenario)
{
  SequencerModel model;
  model.SetUndoEnabled(true, 100);

  auto procedure =
      model.InsertItem<ProcedureItem>(model.GetProcedureContainer(), mvvm::TagIndex::Append());

  auto instruction_container = procedure->GetInstructionContainer();

  auto sequence0 =
      InsertInstruction(domainconstants::kSequenceInstructionType, instruction_container);
  auto sequence1 =
      InsertInstruction(domainconstants::kSequenceInstructionType, instruction_container);
  auto sequence2 =
      InsertInstruction(domainconstants::kSequenceInstructionType, instruction_container);

  sequence1->SetIsRootFlag(true);

  auto root_property0 = GetIsRootItem(*sequence0);
  auto root_property1 = GetIsRootItem(*sequence1);
  auto root_property2 = GetIsRootItem(*sequence2);

  ExclusiveCheckStatePresentationItem presentation0(root_property0);
  const ExclusiveCheckStatePresentationItem presentation1(root_property1);
  const ExclusiveCheckStatePresentationItem presentation2(root_property2);

  // validate middle item
  EXPECT_EQ(presentation0.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
  EXPECT_EQ(presentation1.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

  // check first item
  EXPECT_TRUE(presentation0.SetData(QVariant(Qt::Checked), Qt::CheckStateRole));

  EXPECT_EQ(presentation0.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
  EXPECT_EQ(presentation1.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

  // undo
  model.GetCommandStack()->Undo();

  EXPECT_EQ(presentation0.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
  EXPECT_EQ(presentation1.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

  // redo
  model.GetCommandStack()->Redo();
  EXPECT_EQ(presentation0.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
  EXPECT_EQ(presentation1.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
}

}  // namespace oac_tree_gui::test
