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

#ifndef OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_
#define OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_

#include <oac-tree-gui/composer/instruction_editor_context.h>

#include <QObject>

namespace mvvm
{
class SessionItem;
class TagIndex;
class ISessionModel;
}  // namespace mvvm

namespace sup::gui
{
class QueryResult;
}

namespace oac_tree_gui
{

class SequencerModel;
class InstructionContainerItem;

/**
 * @brief The InstructionEditorActionHandler class implements logic to add/remove instructions in
 * InstructionContainerItem.
 *
 * The handler uses callbacks to retrieve current selection and thus deduce where to insert and what
 * to remove.
 */
class InstructionEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit InstructionEditorActionHandler(InstructionEditorContext context,
                                          QObject* parent_object = nullptr);
  ~InstructionEditorActionHandler() override;

  /**
   * @brief Checks if SessionItem of the given type can be inserted after currently selected
   * instruction.
   */
  bool CanInsertAfter(const std::string& item_type) const;

  /**
   * @brief Inserts new instruction of given type after the current selection.
   *
   * @param item_type The item type name.
   */
  void OnInsertInstructionAfterRequest(const std::string& item_type);

  /**
   * @brief Checks if SessionItem of the given type can be inserted into currently selected
   * instruction.
   */
  bool CanInsertInto(const std::string& item_type) const;

  /**
   * @brief Inserts new instruction of given type after the current selection.
   *
   * @param item_type The item type name.
   */
  void OnInsertInstructionIntoRequest(const std::string& item_type);

  /**
   * @brief Checks if currently selected instruction can be removed.
   */
  bool CanRemoveInstruction() const;

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
   * @brief Cut selected instruction.
   */
  void Cut();

  /**
   * @brief Checks if copy operation is possible.
   */
  bool CanCopy() const;

  /**
   * @brief Copy selected instruction.
   */
  void Copy();

  /**
   * @brief Checks if paste-after operation is possible.
   *
   * Paste-after operation inserts new item after the current selection.
   */
  bool CanPasteAfter() const;

  /**
   * @brief Paste new instruction after the current selection.
   */
  void PasteAfter();

  /**
   * @brief Checks if paste-into operation is possible.
   *
   * Paste-into operation appends a new child to existing children of currently selected
   * instruction.
   */
  bool CanPasteInto() const;

  /**
   * @brief Paste instruction as a child into currently selected instruction.
   *
   * It will be appended to already existing children.
   */
  void PasteInto();

signals:
  void SelectItemRequest(mvvm::SessionItem* item);

private:
  InstructionItem* GetSelectedInstruction() const;

  mvvm::ISessionModel* GetModel() const;
  InstructionContainerItem* GetInstructionContainer() const;
  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {}) const;
  void SendMessage(const sup::gui::MessageEvent& message_event) const;

  const QMimeData* GetMimeData() const;

  /**
   * @brief Checks if SessionItem of the given type can be inserted after the current selection.
   *
   * @param item_type Type of the SessionItem to insert.
   * @return A result of the query with an error flag and message.
   */
  sup::gui::QueryResult CanInsertTypeAfterCurrentSelection(const std::string& item_type) const;

  /**
   * @brief Checks if SessionItem of the given type can be inserted into the current selection.
   *
   * @param item_type Type of the SessionItem to insert.
   * @return A result of the querry with error flag and message.
   */
  sup::gui::QueryResult CanInsertTypeIntoCurrentSelection(const std::string& item_type) const;

  /**
   * @brief Inserts given item after current selection.
   */
  void InsertAfterCurrentSelection(std::unique_ptr<mvvm::SessionItem> item);

  /**
   * @brief Inserts given item into current selection.
   */
  void InsertIntoCurrentSelection(std::unique_ptr<mvvm::SessionItem> item);

  /**
   * @brief Inserts given item in the given parent.
   */
  mvvm::SessionItem* InsertItem(std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem* parent,
                                const mvvm::TagIndex& index);

  InstructionEditorContext m_context;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_
