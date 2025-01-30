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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QList>
#include <QMenu>
#include <QWidget>
#include <memory>

class QMenu;

namespace sup::gui
{
class ProxyAction;
class ActionMenu;
class AppContext;
}  // namespace sup::gui

namespace oac_tree_gui
{

class InstructionEditorActionHandler;

/**
 * @brief The InstructionEditorActions class defines actions related to the contrsution of the
 * Instruction item tree.
 *
 * Belongs to InstructionEditorWidget, action logic is handled by the
 * InstructionEditorActionHandler.
 */
class InstructionEditorActions : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief The ActionKey enum defines keys for all available actions.
   */
  enum class ActionKey : std::uint8_t
  {
    kInsertAfter,
    kInsertInto,
    kRemoveSelected,
    kMoveUp,
    kMoveDown,
    kCut,
    kCopy,
    kPasteAfter,
    kPasteInto,
    kTotalCount
  };

  /**
   * @brief Main c-tor
   *
   * @param An action handler to retrieve status of enabled/disabled actions.
   * @param parent_object Parent Qobject.
   */
  explicit InstructionEditorActions(InstructionEditorActionHandler* handler,
                                    QObject* parent_object = nullptr);
  ~InstructionEditorActions() override;

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

  /**
   * @brief Setup insert/remove, and cut/copy/paste actions in the given menu.
   */
  void SetupMenu(QMenu& menu);

  /**
   * @brief Registers actions for given context.
   *
   * The context is normally provided by the parent widget.
   */
  void RegisterActionsForContext(const sup::gui::AppContext& context);

  /**
   * @brief Update enabled/disabled status of all actions.
   *
   * The method should be called every time we feel that the situation in a tree has changed, and
   * some actions might not be available anymore. The simplest, probably, is to call it every time
   * the user selects some other item in a tree.
   */
  void UpdateEnabledStatus();

private:
  /**
   * @brief Setups all actions related to insert/remove operations.
   */
  void SetupInsertRemoveActions();

  /**
   * @brief Setups all copy-paste actions.
   */
  void SetupCutCopyPasteActions();

  /**
   * @brief Creates an instruction to insert instructions.
   */
  std::unique_ptr<QMenu> CreateInsertMenu();

  /**
   * @brief Setup menu to insert instructions.
   *
   * The function can work with one of two menus, m_insert_after_menu and m_insert_into_menu.
   */
  void AboutToShowInsertMenu();

  InstructionEditorActionHandler* m_action_handler{nullptr};

  std::unique_ptr<QMenu> m_insert_after_menu;
  std::unique_ptr<QMenu> m_insert_into_menu;

  sup::gui::ActionMenu* m_insert_after_action{nullptr};
  sup::gui::ActionMenu* m_insert_into_action{nullptr};
  QAction* m_remove_action{nullptr};
  //!< toolbar version without enable/disable features
  sup::gui::ProxyAction* m_remove_toolbar_action{nullptr};
  QAction* m_move_up_action{nullptr};
  QAction* m_move_down_action{nullptr};

  QAction* m_cut_action{nullptr};
  QAction* m_copy_action{nullptr};
  QAction* m_paste_after_action{nullptr};
  QAction* m_paste_into_action{nullptr};

  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_
