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

#include "oac_tree_gui/pvmonitor/monitor_model.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests for WorkspaceSynchronizer class.

class MonitorModelTest : public ::testing::Test
{
};

TEST_F(MonitorModelTest, InitialState)
{
  MonitorModel model;
  EXPECT_NE(model.GetWorkspaceItem(), nullptr);

  model.Clear();
  EXPECT_NE(model.GetWorkspaceItem(), nullptr);
}

}  // namespace oac_tree_gui::test
