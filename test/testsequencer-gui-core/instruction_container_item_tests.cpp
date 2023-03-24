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

#include "sequencergui/model/instruction_container_item.h"

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

using namespace sequencergui;

class InstructionContainerItemTest : public ::testing::Test
{
};

TEST_F(InstructionContainerItemTest, Clone)
{
  using testutils::IsCloneImplemented;

  EXPECT_TRUE(IsCloneImplemented<InstructionContainerItem>());
}
