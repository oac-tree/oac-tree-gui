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

#include "app_actions.h"

#include "app_action_manager.h"

namespace sequencergui
{

ActionManager &GetGlobalActionManager()
{
  static ActionManager global_action_manager;
  return global_action_manager;
}

void AppRegisterMenuBar(QMenuBar *menubar)
{
  GetGlobalActionManager().SetMenuBar(menubar);
}

IActionContainer *AppAddMenu(const std::string &menu_name)
{
  return GetGlobalActionManager().AddMenu(menu_name);
}

bool AppRegisterAction(const std::string &menu_name, QAction *action)
{
  return GetGlobalActionManager().RegisterAction(menu_name, action);
}

QMenu *AppGetMenu(const std::string &menu_name)
{
  auto container = GetGlobalActionManager().GetContainer(menu_name);
  return container ? container->GetMenu() : nullptr;
}

}  // namespace sequencergui
