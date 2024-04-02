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
#include <sequencergui/components/action_map.h>

class QMenu;

namespace sup::gui
{
class ActionMenu;
}  // namespace sup::gui

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
  enum class ActionKey
  {
    kAdd,
    kRemove,
    kCut,
    kCopy,
    kPaste,
    kTotalCount
  };

  explicit WorkspaceEditorActions(QObject* parent = nullptr);
  ~WorkspaceEditorActions() override;

  QList<QAction*> GetActions(const std::vector<ActionKey> &action_keys) const;

signals:
  void AddVariableRequest(const QString& name);
  void RemoveVariableRequest();
  void CutRequest();
  void CopyRequest();
  void PasteRequest();

private:
  void SetupActions();

  std::unique_ptr<QMenu> CreateInsertAfterMenu();

  std::unique_ptr<QMenu> m_add_variable_menu;
  sup::gui::ActionMenu* m_add_variable_action{nullptr};
  QAction* m_remove_variable_action{nullptr};
  QAction* m_cut_action{nullptr};
  QAction* m_copy_action{nullptr};
  QAction* m_paste_action{nullptr};

  ActionMap<ActionKey> m_action_map;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_ACTIONS_H_
