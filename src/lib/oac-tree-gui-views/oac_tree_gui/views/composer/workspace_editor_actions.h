/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_ACTIONS_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QObject>
#include <memory>

class QMenu;

namespace sup::gui
{
class ActionMenu;
class ProxyAction;
class AppContext;
}  // namespace sup::gui

namespace oac_tree_gui
{

class IWorkspaceEditorActionHandler;

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
  enum class ActionKey : std::uint8_t
  {
    kAdd,
    kRemove,
    kCut,
    kCopy,
    kPaste,
    kTotalCount
  };

  explicit WorkspaceEditorActions(IWorkspaceEditorActionHandler* handler,
                                  QObject* parent_object = nullptr);
  ~WorkspaceEditorActions() override;

  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

  /**
   * @brief Setup cut/copy/paste actions in given menu.
   */
  void SetupMenu(QMenu& menu);

  /**
   * @brief Registers actions for given context.
   *
   * The context is normally provided by the parent widget.
   */
  void RegisterActionsForContext(const sup::gui::AppContext& context);

private:
  void SetupActions();

  std::unique_ptr<QMenu> CreateInsertAfterMenu();

  IWorkspaceEditorActionHandler* m_action_handler{nullptr};

  std::unique_ptr<QMenu> m_add_variable_menu;
  sup::gui::ActionMenu* m_add_variable_action{nullptr};
  QAction* m_remove_variable_action{nullptr};
  //!< toolbar version without enable/disable features
  sup::gui::ProxyAction* m_remove_variable_toolbar_action{nullptr};
  QAction* m_cut_action{nullptr};
  QAction* m_copy_action{nullptr};
  QAction* m_paste_action{nullptr};

  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_ACTIONS_H_
