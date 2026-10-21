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

#include "oac_tree_gui/model/attribute_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/item_utils.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for AttributeItem class.
 */
class AttributeItemTest : public ::testing::Test
{
};

TEST_F(AttributeItemTest, InitialState)
{
  const AttributeItem item;

  EXPECT_FALSE(item.HasData());
  EXPECT_FALSE(mvvm::utils::IsValid(item.GetDefaultValue()));
  EXPECT_FALSE(item.HasFlag(mvvm::Appearance::kNullable));
  EXPECT_FALSE(item.IsTextAllowed());
  EXPECT_EQ(item.GetState(), AttributeState::kNotSet);
  EXPECT_TRUE(item.GetText().empty());
}

TEST_F(AttributeItemTest, SetAttributeType)
{
  {  // nullable attribute has no data
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{42}, /*is_nullable*/ true, /*allow_text*/ true);
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kNullable));
    EXPECT_EQ(item.GetDefaultValue(), mvvm::variant_t{mvvm::uint32{42}});
    EXPECT_EQ(item.GetState(), AttributeState::kNotSet);
    EXPECT_TRUE(item.IsTextAllowed());
  }

  {  // non-nullable attribute: no value set
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{42}, /*is_nullable*/ false, /*allow_text*/ false);
    EXPECT_FALSE(item.HasFlag(mvvm::Appearance::kNullable));
    EXPECT_FALSE(item.IsTextAllowed());
    EXPECT_EQ(item.GetState(), AttributeState::kNotSet);
  }

  {  // string attribute is never nullable and takes no text
    AttributeItem item;
    EXPECT_THROW(item.SetAttributeType(std::string(), /*is_nullable*/ true, /*allow_text*/ false),
                 LogicErrorException);
    EXPECT_THROW(item.SetAttributeType(std::string(), /*is_nullable*/ false, /*allow_text*/ true),
                 LogicErrorException);
    item.SetAttributeType(std::string(), /*is_nullable*/ false, /*allow_text*/ false);
    EXPECT_FALSE(item.HasFlag(mvvm::Appearance::kNullable));
    EXPECT_FALSE(item.IsTextAllowed());
  }
}

TEST_F(AttributeItemTest, SetValue)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);

  EXPECT_TRUE(item.SetData(mvvm::uint32{42}));
  EXPECT_EQ(item.GetState(), AttributeState::kValue);
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});

  // values of other types and strings are refused
  EXPECT_FALSE(item.SetData(mvvm::int32{42}));
  EXPECT_FALSE(item.SetData(42.0));
  EXPECT_FALSE(item.SetData(true));
  EXPECT_FALSE(item.SetData(std::string("$par")));
  EXPECT_FALSE(item.SetValue(mvvm::int32{42}));
  EXPECT_FALSE(item.SetValue(mvvm::variant_t{}));
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});

  EXPECT_TRUE(item.SetValue(mvvm::uint32{43}));
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{43}});

  // no data
  EXPECT_TRUE(item.SetData(mvvm::variant_t{}));
  EXPECT_EQ(item.GetState(), AttributeState::kNotSet);
}

TEST_F(AttributeItemTest, NonNullableRefusesNoData)
{
  AttributeItem item;
  item.SetAttributeType(1.0, /*is_nullable*/ false, /*allow_text*/ true);
  EXPECT_TRUE(item.SetData(1.0));

  EXPECT_FALSE(item.SetData(mvvm::variant_t{}));
  EXPECT_FALSE(item.Unset());
  EXPECT_EQ(item.Data(), mvvm::variant_t{1.0});
}

//! Text sits on top of the value; the value comes back when the text is removed.
TEST_F(AttributeItemTest, TextOverValue)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
  EXPECT_TRUE(item.SetData(mvvm::uint32{42}));

  EXPECT_TRUE(item.SetText("$par"));
  EXPECT_EQ(item.GetState(), AttributeState::kText);
  EXPECT_EQ(item.GetText(), std::string("$par"));
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});

  EXPECT_TRUE(item.SetText("@var"));
  EXPECT_EQ(item.GetText(), std::string("@var"));

  // empty text removes the text
  EXPECT_TRUE(item.SetText(""));
  EXPECT_EQ(item.GetState(), AttributeState::kValue);
  EXPECT_TRUE(item.GetText().empty());
  EXPECT_FALSE(item.HasData(itemconstants::kAttributeTextRole));
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});
}

TEST_F(AttributeItemTest, TextWithoutValue)
{
  {  // nullable attribute returns to not set
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
    EXPECT_TRUE(item.SetText("$par"));
    EXPECT_EQ(item.GetState(), AttributeState::kText);
    EXPECT_TRUE(item.SetText(""));
    EXPECT_EQ(item.GetState(), AttributeState::kNotSet);
  }

  {  // non-nullable attribute returns to its value
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ false, /*allow_text*/ true);
    EXPECT_TRUE(item.SetData(mvvm::uint32{42}));
    EXPECT_TRUE(item.SetText("$par"));
    EXPECT_TRUE(item.SetText(""));
    EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});
  }
}

TEST_F(AttributeItemTest, TextNotAllowed)
{
  AttributeItem item;
  item.SetAttributeType(false, /*is_nullable*/ true, /*allow_text*/ false);
  EXPECT_FALSE(item.IsTextAllowed());

  EXPECT_FALSE(item.SetText("$par"));
  EXPECT_FALSE(item.SetData(std::string("$par"), itemconstants::kAttributeTextRole));
  EXPECT_FALSE(item.SetData(std::string("$par")));
  EXPECT_EQ(item.GetState(), AttributeState::kNotSet);

  EXPECT_TRUE(item.SetData(true));
  EXPECT_FALSE(item.SetText("@var"));
  EXPECT_EQ(item.GetState(), AttributeState::kValue);
}

TEST_F(AttributeItemTest, StringAttribute)
{
  AttributeItem item;
  item.SetAttributeType(std::string(), /*is_nullable*/ false, /*allow_text*/ false);

  // a string attribute holds the placeholder as its value
  EXPECT_TRUE(item.SetData(std::string("$par")));
  EXPECT_EQ(item.GetState(), AttributeState::kValue);
  EXPECT_FALSE(item.SetText("$abc"));

  EXPECT_FALSE(item.SetData(42));
  EXPECT_FALSE(item.SetData(mvvm::variant_t{}));
  EXPECT_EQ(item.Data(), mvvm::variant_t{std::string("$par")});

  // empty string is a value
  EXPECT_TRUE(item.SetData(std::string()));
  EXPECT_EQ(item.GetState(), AttributeState::kValue);
}

TEST_F(AttributeItemTest, SetValueAndUnsetRemoveText)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
  EXPECT_TRUE(item.SetData(mvvm::uint32{42}));
  EXPECT_TRUE(item.SetText("$par"));

  // refused value changes nothing
  EXPECT_FALSE(item.SetValue(42.0));
  EXPECT_EQ(item.GetState(), AttributeState::kText);

  EXPECT_TRUE(item.SetValue(mvvm::uint32{42}));
  EXPECT_EQ(item.GetState(), AttributeState::kValue);
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});

  EXPECT_TRUE(item.SetText("$par"));
  EXPECT_TRUE(item.Unset());
  EXPECT_EQ(item.GetState(), AttributeState::kNotSet);
  EXPECT_FALSE(item.HasData(itemconstants::kAttributeTextRole));
}

TEST_F(AttributeItemTest, OtherRoles)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);

  EXPECT_TRUE(item.SetData(std::string("tooltip"), mvvm::DataRole::kTooltip));
  EXPECT_EQ(item.GetToolTip(), std::string("tooltip"));
}

//! Each operation is one undo step.
TEST_F(AttributeItemTest, Undo)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<AttributeItem>();
  item->SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
  EXPECT_TRUE(item->SetData(mvvm::uint32{42}));

  model.SetUndoEnabled(true);
  auto commands = model.GetCommandStack();

  EXPECT_TRUE(item->SetText("$par"));
  EXPECT_EQ(commands->GetCommandCount(), 1);

  commands->Undo();
  EXPECT_EQ(item->GetState(), AttributeState::kValue);
  commands->Redo();
  EXPECT_EQ(item->GetText(), std::string("$par"));

  // value over text
  EXPECT_TRUE(item->SetValue(mvvm::uint32{43}));
  EXPECT_EQ(commands->GetCommandCount(), 2);

  commands->Undo();
  EXPECT_EQ(item->GetText(), std::string("$par"));
  EXPECT_EQ(item->Data(), mvvm::variant_t{mvvm::uint32{42}});

  // unset over text
  EXPECT_TRUE(item->Unset());
  EXPECT_EQ(commands->GetCommandCount(), 2);
  EXPECT_EQ(item->GetState(), AttributeState::kNotSet);

  commands->Undo();
  EXPECT_EQ(item->GetText(), std::string("$par"));
  EXPECT_EQ(item->Data(), mvvm::variant_t{mvvm::uint32{42}});
}

TEST_F(AttributeItemTest, Clone)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
  EXPECT_TRUE(item.SetData(mvvm::uint32{42}));
  EXPECT_TRUE(item.SetText("$par"));

  auto clone = item.Clone();
  auto attribute_clone = dynamic_cast<AttributeItem*>(clone.get());
  ASSERT_NE(attribute_clone, nullptr);
  EXPECT_EQ(attribute_clone->GetText(), std::string("$par"));
  EXPECT_EQ(attribute_clone->Data(), mvvm::variant_t{mvvm::uint32{42}});
  EXPECT_EQ(attribute_clone->GetDefaultValue(), mvvm::variant_t{mvvm::uint32{0}});
  EXPECT_TRUE(attribute_clone->HasFlag(mvvm::Appearance::kNullable));
  EXPECT_TRUE(attribute_clone->IsTextAllowed());

  // the clone has the data strategy
  EXPECT_FALSE(attribute_clone->SetData(42.0));
  EXPECT_TRUE(attribute_clone->SetText(""));
  EXPECT_EQ(attribute_clone->GetState(), AttributeState::kValue);
}

TEST_F(AttributeItemTest, XmlRoundTrip)
{
  (void)mvvm::RegisterGlobalItem<AttributeItem>();

  {  // text over value
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
    EXPECT_TRUE(item.SetData(mvvm::uint32{42}));
    EXPECT_TRUE(item.SetText("$par"));

    auto reco = mvvm::utils::SessionItemFromXMLString(mvvm::utils::ToXMLString(item));
    auto attribute_reco = dynamic_cast<AttributeItem*>(reco.get());
    ASSERT_NE(attribute_reco, nullptr);
    EXPECT_EQ(attribute_reco->GetText(), std::string("$par"));
    EXPECT_EQ(attribute_reco->Data(), mvvm::variant_t{mvvm::uint32{42}});
    EXPECT_TRUE(attribute_reco->IsTextAllowed());

    // the reconstructed item has the data strategy
    EXPECT_FALSE(attribute_reco->SetData(42.0));
    EXPECT_TRUE(attribute_reco->SetText(""));
    EXPECT_EQ(attribute_reco->GetState(), AttributeState::kValue);
  }

  {  // not set, no text
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ false);

    auto reco = mvvm::utils::SessionItemFromXMLString(mvvm::utils::ToXMLString(item));
    auto attribute_reco = dynamic_cast<AttributeItem*>(reco.get());
    ASSERT_NE(attribute_reco, nullptr);
    EXPECT_EQ(attribute_reco->GetState(), AttributeState::kNotSet);
    EXPECT_EQ(attribute_reco->GetDefaultValue(), mvvm::variant_t{mvvm::uint32{0}});
    EXPECT_TRUE(attribute_reco->HasFlag(mvvm::Appearance::kNullable));
    EXPECT_FALSE(attribute_reco->IsTextAllowed());
    EXPECT_FALSE(attribute_reco->SetText("$par"));
  }
}

}  // namespace oac_tree_gui::test
