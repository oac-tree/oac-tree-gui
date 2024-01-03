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

//! Collection of Qt actions related to the construction of InstructionItem tree.
//! They are used to trigger InstructionEditorController, which does a real job.

class InstructionEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit InstructionEditorActions(QObject* parent = nullptr);
  ~InstructionEditorActions() override;

  QList<QAction*> GetActions() const;

signals:
  void InsertIntoRequest(const QString& name);
  void InsertAfterRequest(const QString& name);
  void RemoveSelectedRequest();
  void MoveUpRequest();
  void MoveDownRequest();

private:
  void SetupActions();

  std::unique_ptr<QMenu> CreateInsertAfterMenu();
  std::unique_ptr<QMenu> CreateInsertIntoMenu();

  std::unique_ptr<QMenu> m_insert_into_menu;
  std::unique_ptr<QMenu> m_insert_after_menu;

  QWidgetAction* m_insert_after_action{nullptr};
  QWidgetAction* m_insert_into_action{nullptr};
  QWidgetAction* m_remove_action{nullptr};
  QWidgetAction* m_move_up_action{nullptr};
  QWidgetAction* m_move_down_action{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_
