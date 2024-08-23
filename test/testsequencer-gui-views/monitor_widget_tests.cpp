/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/views/pvmonitor/monitor_widget.h"

#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_model.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

namespace sequencergui
{

//! Tests of MonitorWidget class.

class MonitorWidgetTest : public ::testing::Test
{
};

TEST_F(MonitorWidgetTest, StartStopStart)
{
  MonitorModel model;

  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 =
      model.GetWorkspaceItem()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item0->SetName("abc");
  SetAnyValue(value0, *variable_item0);

  MonitorWidget widget(&model);

  EXPECT_NO_FATAL_FAILURE(widget.OnStartMonitoringRequest());
  EXPECT_NO_FATAL_FAILURE(widget.OnStopMonitoringRequest());
  EXPECT_NO_FATAL_FAILURE(widget.OnStartMonitoringRequest());
}

}  // namespace sequencergui
