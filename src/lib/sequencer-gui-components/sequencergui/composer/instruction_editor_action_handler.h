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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_

#include <sequencergui/composer/instruction_editor_context.h>

#include <QObject>

namespace mvvm
{
class SessionItem;
class TagIndex;
class SessionModelInterface;
}  // namespace mvvm

namespace sequencergui
{

class SequencerModel;
class InstructionContainerItem;

/**
 * @brief The InstructionEditorActionHandler class implements logic to add/remove instructions in
 * InstructionContainerItem.
 *
 * @details The handler uses callbacks to retrieve current selection and thus deduce where to insert
 * and what to remove.
 */
class InstructionEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit InstructionEditorActionHandler(InstructionEditorContext context,
                                          QObject* parent = nullptr);
  ~InstructionEditorActionHandler() override;

  /**
   * @brief Inserts new instruction of given type after the current selection.
   *
   * @param item_type The item type name.
   */
  void OnInsertInstructionAfterRequest(const QString& item_type);

  /**
   * @brief Inserts new instruction of given type after the current selection.
   *
   * @param item_type The item type name.
   */
  void OnInsertInstructionIntoRequest(const QString& item_type);

  /**
   * @brief Removes currently selected instruction.
   */
  void OnRemoveInstructionRequest();

  /**
   * @brief Moves instruction up (decrement index) in parent's container.
   *
   * @details The method doesn't change a parent, so will do nothing for the first child.
   */
  void OnMoveUpRequest();

  /**
   * @brief Moves instruction down (increment index) in parent's container.
   *
   * @details The method doesn't change a parent, so will do nothing for the last child.
   */
  void OnMoveDownRequest();

  /**
   * @brief Cals external dialog for AnyValueItem editing.
   */
  void OnEditAnyvalueRequest();

  /**
   * @brief Checks if cut operation is possible.
   */
  bool CanCut() const;

  /**
   * @brief Cut selected procedure.
   */
  void Cut();

  /**
   * @brief Checks if copy operation is possible.
   */
  bool CanCopy() const;

  /**
   * @brief Copy selected procedure.
   */
  void Copy();

  /**
   * @brief Checks if paste operation is possible.
   */
  bool CanPasteAfter() const;

  /**
   * @brief Paste instruction after current selection.
   */
  void Paste();

signals:
  void SelectItemRequest(mvvm::SessionItem* item);

private:
  InstructionItem* GetSelectedInstruction() const;

  mvvm::SessionModelInterface* GetModel() const;
  InstructionContainerItem* GetInstructionContainer() const;
  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});
  const QMimeData* GetMimeData() const;

  ::mvvm::SessionItem* InsertItem(const std::string& item_type, ::mvvm::SessionItem* parent,
                                  const ::mvvm::TagIndex& index);

  InstructionEditorContext m_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_
