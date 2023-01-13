/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/instruction_item.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for InstructionItem base class.

class InstructionItemsTest : public ::testing::Test
{
public:
  class TestItem : public InstructionItem
  {
  public:
    TestItem() : InstructionItem("test") {}

    std::string GetDomainType() const override { return "domain_name"; }

  private:
    void InitFromDomainImpl(const instruction_t* instruction) override{};
    void SetupDomainImpl(instruction_t* instruction) const override{};
  };
};

TEST_F(InstructionItemsTest, TestItem)
{
  // Correctly initialised item
  TestItem item;
  EXPECT_EQ(item.GetType(), "test");
  EXPECT_EQ(item.GetName(), std::string());

  EXPECT_EQ(item.GetX(), 0);
  EXPECT_EQ(item.GetY(), 0);

  item.SetX(1.1);
  item.SetY(1.2);
  EXPECT_EQ(item.GetX(), 1.1);
  EXPECT_EQ(item.GetY(), 1.2);

  item.SetName("MyName");
  EXPECT_EQ(item.GetName(), std::string("MyName"));

  EXPECT_EQ(item.GetStatus(), "");
  EXPECT_TRUE(item.GetInstructions().empty());

  EXPECT_FALSE(item.IsRoot());
  item.SetIsRootFlag(true);
  EXPECT_TRUE(item.IsRoot());
}
