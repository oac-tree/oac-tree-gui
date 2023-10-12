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

#include "app_action_manager.h"

#include <sequencergui/core/exceptions.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace sequencergui
{

QMenuBar *ActionManager::GetMenuBar()
{
  return m_menubar;
}

void ActionManager::SetMenuBar(QMenuBar *menubar)
{
  if (m_menubar)
  {
    throw LogicErrorException("Manu bar was already initialised.");
  }

  m_menubar = menubar;
}

IActionContainer *ActionManager::AddMenu(const std::string &menu_name)
{
  ValidateMenuBar();

  if (auto container = GetContainer(menu_name); container)
  {
    return container;
  }

  auto menu = m_menubar->addMenu(QString::fromStdString(menu_name));
  menu->setToolTipsVisible(true);
  auto container = std::make_unique<MenuActionContainer>(menu_name, menu);
  auto container_ptr = container.get();
  m_action_storage.insert({menu_name, std::move(container)});
  return container_ptr;
}

bool ActionManager::RegisterAction(const std::string &menu_name, QAction *action)
{
  if (auto container = GetContainer(menu_name); container)
  {
    return container->AddAction(action);
  }

  return false;
}

IActionContainer *ActionManager::GetContainer(const std::string &menu_name)
{
  auto iter = m_action_storage.find(menu_name);
  return iter == m_action_storage.end() ? nullptr : iter->second.get();
}

size_t ActionManager::GetContainerCount() const
{
  return m_action_storage.size();
}

void ActionManager::ValidateMenuBar()
{
  if (!m_menubar)
  {
    throw LogicErrorException("No menubar was initialised");
  }
}

}  // namespace sequencergui
