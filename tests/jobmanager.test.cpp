/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/jobmanager.h"

#include <gtest/gtest.h>

using namespace sequi;

//! Tests for JobManager.

class JobManagerTest : public ::testing::Test
{
};

TEST_F(JobManagerTest, InitialState)
{
  EXPECT_EQ(1, 1);

}
