/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "sequencergui/model/settings_model.h"

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

using namespace sequencergui;

//! Tests for SettingsModel class.
class SettingsModelTest : public ::testing::Test
{
};

TEST_F(SettingsModelTest, InitialState)
{
  SettingsModel model;

  EXPECT_EQ(model.GetSettingsItems().size(), 1);

  EXPECT_EQ(model.Data<bool>(kUseUndoSetting), kUseUndoDefault);
  EXPECT_EQ(model.Data<int>(kUndoLimitSetting), kUndoLimitDefault);
}

TEST_F(SettingsModelTest, Clear)
{
  SettingsModel model;

  model.GetSettingsItems().at(0)->SetProperty(kUndoLimitSetting, 42);
  EXPECT_EQ(model.Data<int>(kUndoLimitSetting), 42);
  model.Clear();

  // after clearing all setting items, and their values have been re-initialised
  EXPECT_EQ(model.GetSettingsItems().size(), 1);
  EXPECT_EQ(model.Data<bool>(kUseUndoSetting), kUseUndoDefault);
  EXPECT_EQ(model.Data<int>(kUndoLimitSetting), kUndoLimitDefault);
}

