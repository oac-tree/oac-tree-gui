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

#ifndef SEQUENCERGUI_MAINWINDOW_APP_ACTIONS_H_
#define SEQUENCERGUI_MAINWINDOW_APP_ACTIONS_H_

//! @file
//! Collection of helper method to register global actions.

#include <QList>
#include <string>

class QMenuBar;
class QAction;
class QMenu;

namespace sequencergui
{

namespace constants
{
//!< the name of the groups corresponding to main menus of the menubar

static inline const std::string kFileMenu = "&File";
static inline const std::string kViewMenu = "&View";
static inline const std::string kToolsMenu = "&Tools";
static inline const std::string kHelpMenu = "&Help";
}  // namespace app

class ActionManager;
class IActionContainer;

/**
 * @brief Returns global action manager.
 */
ActionManager& GetGlobalActionManager();

/**
 * @brief Registers the main menubar of QMainWindow to use in action manager.
 */
void AppRegisterMenuBar(QMenuBar* menubar);

/**
 * @brief Adds menu to the registered menubar.
 */
IActionContainer* AppAddMenu(const std::string& menu_name);

/**
 * @brief Register and add action to the menu.
 */
bool AppRegisterAction(const std::string& menu_name, QAction* action);

/**
 * @brief Register and add action to the menu.
 */
bool AppRegisterActions(const std::string& menu_name, const QList<QAction*>& actions);

/**
 * @brief Returns top level menu registered in a menubar under this name.
 */
QMenu* AppGetMenu(const std::string& menu_name);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_APP_ACTIONS_H_
