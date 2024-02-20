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

#ifndef SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_ACTIONS_H_
#define SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_ACTIONS_H_

#include <QObject>
#include <memory>

class QMenu;
class QWidgetAction;
class QAction;

namespace sequencergui
{

/**
 * @brief The WorkspaceEditorActions class holds collection of Qt actions to construct variables in
 * the workspace.
 *
 * The actual job is done by WorkspaceEditorActionHandler.
 */
class WorkspaceEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit WorkspaceEditorActions(QObject* parent = nullptr);
  ~WorkspaceEditorActions() override;

  QList<QAction*> GetActions() const;

signals:
  void AddVariableRequest(const QString& name);
  void RemoveVariableRequest();

private:
  void SetupActions();

  std::unique_ptr<QMenu> CreateInsertAfterMenu();

  std::unique_ptr<QMenu> m_add_variable_menu;
  QWidgetAction* m_add_variable_action{nullptr};
  QWidgetAction* m_remove_variable_action{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_ACTIONS_H_
