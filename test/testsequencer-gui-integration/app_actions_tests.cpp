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

#include "sequencergui/mainwindow/app_actions.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/mainwindow/app_action_manager.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

using namespace sequencergui;

class AppActionsTest : public ::testing::Test
{
};

TEST_F(AppActionsTest, ActionManagerAddMenu)
{
  QMenuBar menubar;

  AppRegisterMenuBar(&menubar);

  auto container = AppAddMenu("File");

  // There is no way to check if menubar got a menu. We can check only an action associated with
  // the menu.
  EXPECT_EQ(menubar.actions().size(), 1);

  // We can get access to the menu via container. Menu was created, it has no actions yet.
  EXPECT_TRUE(container->GetMenu());
  EXPECT_EQ(container->GetMenu(), AppGetMenu("File"));
  EXPECT_EQ(container->GetActionCount(), 0);

  QAction action;
  EXPECT_TRUE(AppRegisterAction("File", &action));
}
