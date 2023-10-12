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

#include "sequencergui/mainwindow/app_action_manager.h"

#include <sequencergui/core/exceptions.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

using namespace sequencergui;

class AppActionManagerTest : public ::testing::Test
{
};

TEST_F(AppActionManagerTest, MenuActionContainer)
{
  QMenu menu;

  MenuActionContainer container("abc", &menu);
  EXPECT_EQ(container.GetActionCount(), 0);
  EXPECT_EQ(container.GetMenu(), &menu);
  EXPECT_EQ(container.GetName(), std::string("abc"));

  QAction action;
  EXPECT_TRUE(container.AddAction(&action));
  EXPECT_EQ(container.GetActionCount(), 1);
}

TEST_F(AppActionManagerTest, ActionManagerInitialState)
{
  ActionManager manager;
  EXPECT_EQ(manager.GetContainerCount(), 0);
  EXPECT_EQ(manager.GetContainer(ActionManager::kViewMenu), nullptr);
  EXPECT_EQ(manager.GetMenuBar(), nullptr);
}

TEST_F(AppActionManagerTest, ActionManagerSetMenuBar)
{
  ActionManager manager;
  QMenuBar menubar;

  manager.SetMenuBar(&menubar);
  EXPECT_EQ(manager.GetMenuBar(), &menubar);
  EXPECT_THROW(manager.SetMenuBar(&menubar), LogicErrorException);
}

TEST_F(AppActionManagerTest, ActionManagerAddMenu)
{
  ActionManager manager;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  manager.SetMenuBar(&menubar);

  // creating container with the menu
  auto container = manager.AddMenu("File");
  EXPECT_EQ(manager.GetContainerCount(), 1);
  EXPECT_EQ(manager.GetContainer("File"), container);

  // there is no harm to try to add the menu second time
  auto container2 = manager.AddMenu("File");
  EXPECT_EQ(container, container2);
  EXPECT_EQ(manager.GetContainerCount(), 1);

  // There is no way to check if menubar got a menu. We can check opnly an action associated with
  // the menu.
  EXPECT_EQ(menubar.actions().size(), 1);

  // We can get access to the menu via container. Menu was created, it has no actions yet.
  EXPECT_TRUE(container->GetMenu());
  EXPECT_EQ(container->GetActionCount(), 0);
}

TEST_F(AppActionManagerTest, ActionManagerRegisterAction)
{
  ActionManager manager;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  manager.SetMenuBar(&menubar);
  auto container = manager.AddMenu("File");

  // Registering user action.
  QAction action;
  EXPECT_TRUE(manager.RegisterAction("File", &action));

  // for the moment it is possible to add same action twice
  EXPECT_TRUE(manager.RegisterAction("File", &action));

  // it is not possible to add action to not-registered menu
  EXPECT_FALSE(manager.RegisterAction("Edit", &action));
}
