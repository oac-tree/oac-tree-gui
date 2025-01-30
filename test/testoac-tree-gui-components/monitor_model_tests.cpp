/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <gtest/gtest.h>

#include "oac-tree-gui/pvmonitor/monitor_model.h"

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
}

}  // namespace oac_tree_gui::test
