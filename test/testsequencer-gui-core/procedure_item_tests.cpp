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

#include "sequencergui/model/procedure_item.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Testing ProcedureItem class.

class ProcedureItemTest : public ::testing::Test
{
};

TEST_F(ProcedureItemTest, InitialState)
{
  ProcedureItem item;

  EXPECT_TRUE(item.GetInstructionContainer());
  EXPECT_TRUE(item.GetWorkspace());
  EXPECT_TRUE(item.GetPreambleItem());
}

TEST_F(ProcedureItemTest, GetterAndSetter)
{
  ProcedureItem item;

  EXPECT_EQ(item.GetName(), std::string());
  EXPECT_EQ(item.GetDescription(), std::string());
  EXPECT_EQ(item.GetFileName(), std::string());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetDescription("def");
  EXPECT_EQ(item.GetDescription(), std::string("def"));

  item.SetFileName("jhk");
  EXPECT_EQ(item.GetFileName(), std::string("jhk"));
}
