/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "suppvmonitor/workspace_controller.h"

#include <gtest/gtest.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/anyvalue_utils.h>
#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/workspace.h>
#include <suppvmonitor/monitor_model.h>

#include <QTest>

using namespace suppvmonitor;

//! Tests for WorkspaceController class.

class WorkspaceControllerSoftIocTest : public ::testing::Test
{
};

TEST_F(WorkspaceControllerSoftIocTest, InitialState)
{
  MonitorModel model;
  WorkspaceController controller(&model);
  EXPECT_EQ(1, 1);
}
