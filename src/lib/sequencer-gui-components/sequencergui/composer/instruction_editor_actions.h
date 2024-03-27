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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_

#include <QList>
#include <QMenu>
#include <QWidget>
#include <memory>

class QAction;
class QWidgetAction;
class QMenu;

namespace sequencergui
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
  enum class ActionKey
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
   * @param parent Parent Qobject.
   */
  explicit InstructionEditorActions(const InstructionEditorActionHandler* handler, QObject* parent = nullptr);
  ~InstructionEditorActions() override;

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys);

  /**
   * @brief Returns action for given key.
   */
  QAction* GetAction(ActionKey key) const;

  /**
   * @brief Setup cut/copy/paste actions in given menu.
   */
  void SetupMenu(QMenu& menu);

signals:
  void InsertIntoRequest(const QString& name);
  void InsertAfterRequest(const QString& name);
  void RemoveSelectedRequest();
  void MoveUpRequest();
  void MoveDownRequest();
  void CutRequest();
  void CopyRequest();
  void PasteAfterRequest();
  void PasteIntoRequest();

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
   * @brief Creates an instruction to insert an instruction after currently selected instruction.
   */
  std::unique_ptr<QMenu> CreateInsertAfterMenu() const;

  /**
   * @brief Populates content of m_insert_after_menu with actions relevant to currently selected
   * instruction.
   */
  void OnAboutToShowInsertAfterMenu();

  /**
   * @brief Creates menu to insert an instruction into currently selected instruction.
   */
  std::unique_ptr<QMenu> CreateInsertIntoMenu() const;

  /**
   * @brief Populates content of m_insert_into_menu with actions relevant to currently selected
   * instruction.
   */
  void OnAboutToShowInsertIntoMenu();

  const InstructionEditorActionHandler* m_handler{nullptr};

  std::unique_ptr<QMenu> m_insert_after_menu;
  std::unique_ptr<QMenu> m_insert_into_menu;

  QAction* m_insert_after_action{nullptr};
  QWidgetAction* m_insert_after_toolbar_action{nullptr};  //!< toolbar version with instant popup
  QAction* m_insert_into_action{nullptr};
  QWidgetAction* m_insert_into_toolbar_action{nullptr};  //!< toolbar version with instant popup
  QAction* m_remove_action{nullptr};
  QAction* m_remove_toolbar_action{nullptr};  //!< toolbar version without enable/disable features
  QAction* m_move_up_action{nullptr};
  QAction* m_move_down_action{nullptr};

  QAction* m_cut_action{nullptr};
  QAction* m_copy_action{nullptr};
  QAction* m_paste_after_action{nullptr};
  QAction* m_paste_into_action{nullptr};

  std::map<ActionKey, QAction*> m_actions;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_
