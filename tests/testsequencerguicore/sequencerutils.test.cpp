/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/sequencerutils.h"

#include "sequencergui/model/sequenceritems.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for SequencerObserver class.

class SequencerUtilsTest : public ::testing::Test
{
};

TEST_F(SequencerUtilsTest, IsCompoundInstruction)
{
  SequenceItem sequence;
  EXPECT_TRUE(IsCompoundInstruction(&sequence));

  RepeatItem repeat;
  EXPECT_TRUE(IsCompoundInstruction(&repeat));

  WaitItem wait;
  EXPECT_FALSE(IsCompoundInstruction(&wait));
}

TEST_F(SequencerUtilsTest, IsDecoratorInstruction)
{
  SequenceItem sequence;
  EXPECT_FALSE(IsDecoratorInstruction(&sequence));

  RepeatItem repeat;
  EXPECT_TRUE(IsDecoratorInstruction(&repeat));

  WaitItem wait;
  EXPECT_FALSE(IsDecoratorInstruction(&wait));
}
