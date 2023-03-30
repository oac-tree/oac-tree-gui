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

#include "sequencergui/pvmonitor/monitor_model.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for WorkspaceSyncronizer class.

class MonitorModelTests : public ::testing::Test
{
};

TEST_F(MonitorModelTests, InitialState)
{
  MonitorModel model;
  EXPECT_TRUE(model.GetWorkspaceItem() == nullptr);
}
