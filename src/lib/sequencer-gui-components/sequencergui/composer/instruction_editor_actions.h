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
    kPaste,
    kTotalCount
  };

  explicit InstructionEditorActions(QObject* parent = nullptr);
  ~InstructionEditorActions() override;

  QList<QAction*> GetActions() const;

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys);

  /**
   * @brief Returns action for given key.
   */
  QAction* GetAction(ActionKey key) const;

signals:
  void InsertIntoRequest(const QString& name);
  void InsertAfterRequest(const QString& name);
  void RemoveSelectedRequest();
  void MoveUpRequest();
  void MoveDownRequest();
  void CutRequest();
  void CopyRequest();
  void PasteRequest();

private:
  void SetupInsertRemoveActions();
  void SetupCutCopyPasteActions();

  /**
   * @brief Creates an instruction to insert an instruction after currently selected instruction.
   */
  std::unique_ptr<QMenu> CreateInsertAfterMenu();

  /**
   * @brief Creates menu to insert an instruction into currently selected instruction.
   */
  std::unique_ptr<QMenu> CreateInsertIntoMenu();

  std::unique_ptr<QMenu> m_insert_after_menu;
  std::unique_ptr<QMenu> m_insert_into_menu;

  QWidgetAction* m_insert_after_action{nullptr};
  QWidgetAction* m_insert_into_action{nullptr};
  QWidgetAction* m_remove_action{nullptr};
  QWidgetAction* m_move_up_action{nullptr};
  QWidgetAction* m_move_down_action{nullptr};

  QAction* m_cut_action{nullptr};
  QAction* m_copy_action{nullptr};
  QAction* m_paste_action{nullptr};

  std::map<ActionKey, QAction*> m_actions;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_
