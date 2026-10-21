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
  EXPECT_FALSE(item.HasText());
  EXPECT_FALSE(item.IsTextAllowed());
}

TEST_F(AttributeItemTest, SetAttributeType)
{
  {  // nullable attribute has no data
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{42}, /*is_nullable*/ true, /*allow_text*/ true);
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kNullable));
    EXPECT_EQ(item.GetDefaultValue(), mvvm::variant_t{mvvm::uint32{42}});
    EXPECT_FALSE(item.HasData());
    EXPECT_TRUE(item.IsTextAllowed());
  }

  {  // non-nullable attribute gets the default value
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{42}, /*is_nullable*/ false, /*allow_text*/ true);
    EXPECT_FALSE(item.HasFlag(mvvm::Appearance::kNullable));
    EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});
  }

  {  // string attribute is never nullable and takes no text
    AttributeItem item;
    item.SetAttributeType(std::string(), /*is_nullable*/ true, /*allow_text*/ true);
    EXPECT_FALSE(item.HasFlag(mvvm::Appearance::kNullable));
    EXPECT_FALSE(item.IsTextAllowed());
    EXPECT_EQ(item.Data(), mvvm::variant_t{std::string()});
  }
}

TEST_F(AttributeItemTest, SetValueAndText)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);

  // value of the attribute type
  EXPECT_TRUE(item.SetData(mvvm::uint32{42}));
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});
  EXPECT_FALSE(item.HasText());

  // values of other types are refused
  EXPECT_FALSE(item.SetData(mvvm::int32{42}));
  EXPECT_FALSE(item.SetData(42.0));
  EXPECT_FALSE(item.SetData(true));
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});

  // text
  EXPECT_TRUE(item.SetData(std::string("$par")));
  EXPECT_EQ(item.Data(), mvvm::variant_t{std::string("$par")});
  EXPECT_TRUE(item.HasText());

  // back to value
  EXPECT_TRUE(item.SetData(mvvm::uint32{43}));
  EXPECT_FALSE(item.HasText());

  // no data
  EXPECT_TRUE(item.SetData(mvvm::variant_t{}));
  EXPECT_FALSE(item.HasData());
  EXPECT_FALSE(item.HasText());
}

TEST_F(AttributeItemTest, NonNullableRefusesNoData)
{
  AttributeItem item;
  item.SetAttributeType(1.0, /*is_nullable*/ false, /*allow_text*/ true);

  EXPECT_FALSE(item.SetData(mvvm::variant_t{}));
  EXPECT_EQ(item.Data(), mvvm::variant_t{1.0});

  // text is accepted
  EXPECT_TRUE(item.SetData(std::string("@var")));
  EXPECT_TRUE(item.HasText());
}

TEST_F(AttributeItemTest, TextNotAllowed)
{
  AttributeItem item;
  item.SetAttributeType(false, /*is_nullable*/ true, /*allow_text*/ false);
  EXPECT_FALSE(item.IsTextAllowed());

  EXPECT_FALSE(item.SetData(std::string("$par")));
  EXPECT_FALSE(item.SetData(std::string()));
  EXPECT_FALSE(item.HasData());

  EXPECT_TRUE(item.SetData(true));
  EXPECT_FALSE(item.SetData(std::string("@var")));
  EXPECT_EQ(item.Data(), mvvm::variant_t{true});

  EXPECT_TRUE(item.SetData(mvvm::variant_t{}));
  EXPECT_FALSE(item.HasData());
}

TEST_F(AttributeItemTest, EmptyText)
{
  {  // nullable attribute: empty text removes the data
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
    EXPECT_TRUE(item.SetData(std::string("$par")));
    EXPECT_TRUE(item.SetData(std::string()));
    EXPECT_FALSE(item.HasData());
  }

  {  // non-nullable attribute: empty text sets the default value
    AttributeItem item;
    item.SetAttributeType(mvvm::uint32{42}, /*is_nullable*/ false, /*allow_text*/ true);
    EXPECT_TRUE(item.SetData(std::string("$par")));
    EXPECT_TRUE(item.SetData(std::string()));
    EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});
  }

  {  // string attribute keeps empty string
    AttributeItem item;
    item.SetAttributeType(std::string(), /*is_nullable*/ false, /*allow_text*/ false);
    EXPECT_TRUE(item.SetData(std::string("abc")));
    EXPECT_TRUE(item.SetData(std::string()));
    EXPECT_EQ(item.Data(), mvvm::variant_t{std::string()});
  }
}

TEST_F(AttributeItemTest, StringAttribute)
{
  AttributeItem item;
  item.SetAttributeType(std::string(), /*is_nullable*/ false, /*allow_text*/ false);

  EXPECT_TRUE(item.SetData(std::string("$par")));
  EXPECT_FALSE(item.HasText());

  EXPECT_FALSE(item.SetData(42));
  EXPECT_FALSE(item.SetData(mvvm::variant_t{}));
  EXPECT_EQ(item.Data(), mvvm::variant_t{std::string("$par")});
}

TEST_F(AttributeItemTest, OtherRoles)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);

  EXPECT_TRUE(item.SetData(std::string("tooltip"), mvvm::DataRole::kTooltip));
  EXPECT_EQ(item.GetToolTip(), std::string("tooltip"));
}

TEST_F(AttributeItemTest, UndoSwitchBetweenValueAndText)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<AttributeItem>();
  item->SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
  EXPECT_TRUE(item->SetData(mvvm::uint32{42}));

  model.SetUndoEnabled(true);
  auto commands = model.GetCommandStack();

  // value to text is one undo step
  EXPECT_TRUE(item->SetData(std::string("$par")));
  EXPECT_EQ(commands->GetCommandCount(), 1);

  commands->Undo();
  EXPECT_EQ(item->Data(), mvvm::variant_t{mvvm::uint32{42}});

  commands->Redo();
  EXPECT_EQ(item->Data(), mvvm::variant_t{std::string("$par")});

  // no data is one undo step
  EXPECT_TRUE(item->SetData(mvvm::variant_t{}));
  EXPECT_EQ(commands->GetCommandCount(), 2);

  commands->Undo();
  EXPECT_EQ(item->Data(), mvvm::variant_t{std::string("$par")});
}

TEST_F(AttributeItemTest, Clone)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);
  EXPECT_TRUE(item.SetData(std::string("$par")));

  auto clone = item.Clone();
  auto attribute_clone = dynamic_cast<AttributeItem*>(clone.get());
  ASSERT_NE(attribute_clone, nullptr);
  EXPECT_EQ(attribute_clone->Data(), mvvm::variant_t{std::string("$par")});
  EXPECT_EQ(attribute_clone->GetDefaultValue(), mvvm::variant_t{mvvm::uint32{0}});
  EXPECT_TRUE(attribute_clone->HasFlag(mvvm::Appearance::kNullable));
  EXPECT_TRUE(attribute_clone->IsTextAllowed());

  // the clone has the data strategy
  EXPECT_FALSE(attribute_clone->SetData(42.0));
  EXPECT_TRUE(attribute_clone->SetData(mvvm::uint32{42}));
}

TEST_F(AttributeItemTest, XmlRoundTrip)
{
  (void)mvvm::RegisterGlobalItem<AttributeItem>();

  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ false);

  auto reco = mvvm::utils::SessionItemFromXMLString(mvvm::utils::ToXMLString(item));
  auto attribute_reco = dynamic_cast<AttributeItem*>(reco.get());
  ASSERT_NE(attribute_reco, nullptr);
  EXPECT_FALSE(attribute_reco->HasData());
  EXPECT_EQ(attribute_reco->GetDefaultValue(), mvvm::variant_t{mvvm::uint32{0}});
  EXPECT_TRUE(attribute_reco->HasFlag(mvvm::Appearance::kNullable));
  EXPECT_FALSE(attribute_reco->IsTextAllowed());

  // the reconstructed item has the data strategy
  EXPECT_FALSE(attribute_reco->SetData(42.0));
  EXPECT_FALSE(attribute_reco->SetData(std::string("$par")));
  EXPECT_TRUE(attribute_reco->SetData(mvvm::uint32{42}));
}

}  // namespace oac_tree_gui::test
