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

#include <oac_tree_gui/composer/i_instruction_editor_action_handler.h>
#include <oac_tree_gui/composer/instruction_editor_context.h>

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
class InstructionEditorActionHandler : public IInstructionEditorActionHandler
{
public:
  explicit InstructionEditorActionHandler(InstructionEditorContext context);
  ~InstructionEditorActionHandler() override;

  void DropInstruction(const std::string& item_type, const position_t& pos) override;

  bool CanInsertInstructionAfter(const std::string& item_type) const override;

  void InsertInstructionAfter(const std::string& item_type) override;

  bool CanInsertInstructionInto(const std::string& item_type) const override;

  void InsertInstructionInto(const std::string& item_type) override;

  bool CanRemoveInstruction() const override;

  void RemoveInstruction() override;

  void MoveUp() override;

  void MoveDown() override;

  void OnEditAnyvalueRequest() override;

  bool CanCut() const override;

  void Cut() override;

  bool CanCopy() const override;

  void Copy() override;

  bool CanPasteAfter() const override;

  void PasteAfter() override;

  bool CanPasteInto() const override;

  void PasteInto() override;

  void InsertItem(const std::string& item_type, mvvm::SessionItem* parent,
                  const mvvm::TagIndex& index) override;

private:
  InstructionItem* GetSelectedInstruction() const;
  mvvm::ISessionModel* GetModel() const;
  mvvm::SessionItem* GetInstructionContainer() const;
  void SelectNotify(mvvm::SessionItem* item) const;

  /**
   * @brief Creates a single instruction or complex instruction aggregate.
   */
  std::unique_ptr<InstructionItem> CreateInstructionTree(const std::string& item_type);

  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {}) const;
  void SendMessage(const sup::gui::MessageEvent& message_event) const;

  const QMimeData* GetMimeData() const;

  /**
   * @brief Updates procedure's plugin name preamble required for the current instruction list.
   */
  void UpdateProcedurePreamble();

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

  mvvm::SessionItem* InsertItem(std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem* parent,
                                const mvvm::TagIndex& index, const position_t& position);

  InstructionEditorContext m_context;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_
