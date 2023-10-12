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

#ifndef SEQUENCERGUI_MAINWINDOW_APP_ACTION_MANAGER_H_
#define SEQUENCERGUI_MAINWINDOW_APP_ACTION_MANAGER_H_

#include <QString>
#include <QVector>
#include <map>
#include <memory>
#include <string>
#include <vector>

class QAction;
class QMenuBar;
class QMenu;

namespace sequencergui
{

class IActionContainer
{
public:
  virtual ~IActionContainer() = default;

  /**
   * @brief Returns number of actions registered in this container.
   */
  virtual size_t GetActionCount() = 0;

  /**
   * @brief The menu associated with this container, where actionms are added.
   */
  virtual QMenu* GetMenu() = 0;

  /**
   * @brief Adds action to the menu associated with this container.
   *
   * @param The action to add (ownership belongs to the user).
   * @return True if action has been successfully added.
   */
  virtual bool AddAction(QAction* action) = 0;
};

/**
 * @brief The MenuActionContainer class holds information about a menu and actions added to it.
 */
class MenuActionContainer : public IActionContainer
{
public:
  /**
   * @brief Main constructor.
   *
   * @param name The name of the container.
   * @param menu The menu to serve.
   */
  explicit MenuActionContainer(const std::string& name, QMenu* menu) : m_menu(menu), m_name(name) {}

  std::string GetName() { return m_name; }

  QMenu* GetMenu() override { return m_menu; }

  bool AddAction(QAction* action) override { m_actions.push_back(action); return true;}

  size_t GetActionCount() override  { return m_actions.size(); }

private:
  std::string m_name;
  QMenu* m_menu{nullptr};
  std::vector<QAction*> m_actions;
};

class IActionManager
{
public:
  virtual ~IActionManager() = default;
  virtual bool RegisterAction(const std::string& group_name, QAction* action) = 0;
};

/**
 * @brief The ActionManager class provides subscriptions of user actions to the menubar of the main
 * window.
 *
 * @details It doesn't own user actions. Intended to be used as a global variable.
 */

class ActionManager : public IActionManager
{
public:
  //!< the name of the group corresponding to main menus of the menubar

  static inline const std::string kFileMenu = "&File";
  static inline const std::string kViewMenu = "&View";
  static inline const std::string kToolsMenu = "&Tools";
  static inline const std::string kHelpMenu = "&Help";

  /**
   * @brief Returns menubar served by this manager.
   */
  QMenuBar* GetMenuBar();

  /**
   * @brief Sets menubar to serve.
   *
   * @details There can be only one menubar to serve.
   */
  void SetMenuBar(QMenuBar* menubar);

  /**
   * @brief Adds new menu to the menu bar.
   *
   * @param menu_name The title of the menu.
   * @param menubar The menubar where the menu will be added.
   *
   * @return Action container holding just created menu and other information.
   *
   * @details The menubar will take an ownership. If menu was already created, just returns action
   * container.
   */
  IActionContainer* AddMenu(const std::string& menu_name);

  /**
   * @brief Registers action and add it to the menu.
   *
   * @param menu_name The name of already existing menu added via AddMenu call.
   * @param action User action to add.
   * @return True if action was added, or false if no such menu was registered.
   *
   * @details The ownership of the action is not taken.
   */
  bool RegisterAction(const std::string& menu_name, QAction* action) override;

  IActionContainer* GetContainer(const std::string& menu_name);

  size_t GetContainerCount() const;

private:
  void ValidateMenuBar();

  std::map<std::string, std::unique_ptr<IActionContainer>> m_action_storage;
  QMenuBar* m_menubar{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_APP_ACTION_MANAGER_H_
