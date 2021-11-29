/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/sequencerobserver.h"

#include <gtest/gtest.h>

using namespace sequi;

//! Tests for SequencerObserver class.

class SequencerObserverTest : public ::testing::Test
{
};

TEST_F(SequencerObserverTest, InitialState)
{
  EXPECT_EQ(1, 1);
}
