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

namespace sequencergui
{

static inline const std::string kViewGroup = "kViewGroup";

class IActionContainer
{
public:
};

class MenuActionContainer : public IActionContainer
{
public:
  explicit MenuActionContainer(const QString& name) : m_name(name) {}

  QString GetName() { return m_name; }

  void AddAction(QAction* action) { m_actions.push_back(action); }

private:
  QString m_name;
  std::vector<QAction*> m_actions;
};

class IActionManager
{
public:
  virtual bool RegisterAction(const std::string& group_name, QAction* action) = 0;
};

class ActionManager : public IActionManager
{
public:
  bool RegisterAction(const std::string& menu_name, QAction* action) override;

private:
  std::map<std::string, std::unique_ptr<IActionContainer>> m_action_storage;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_APP_ACTION_MANAGER_H_
