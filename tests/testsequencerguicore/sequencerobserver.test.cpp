/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
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

#include "sequencergui/monitor/sequencerobserver.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for SequencerObserver class.

class SequencerObserverTest : public ::testing::Test
{
};

TEST_F(SequencerObserverTest, InitialState)
{
  EXPECT_EQ(1, 1);
}
