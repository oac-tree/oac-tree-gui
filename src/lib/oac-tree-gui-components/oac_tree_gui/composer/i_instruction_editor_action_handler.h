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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_COMPOSER_I_INSTRUCTION_EDITOR_ACTION_HANDLER_H_
#define OAC_TREE_GUI_COMPOSER_I_INSTRUCTION_EDITOR_ACTION_HANDLER_H_

#include <string>
#include <utility>

namespace mvvm
{
class SessionItem;
class TagIndex;
class ISessionModel;
}  // namespace mvvm

namespace oac_tree_gui
{

/**
 * @brief The InstructionEditorActionHandler class implements logic to add/remove instructions in
 * instruction container.
 */
class IInstructionEditorActionHandler
{
public:
  using position_t = std::pair<double, double>;

  IInstructionEditorActionHandler() = default;
  virtual ~IInstructionEditorActionHandler() = default;

  IInstructionEditorActionHandler(const IInstructionEditorActionHandler&) = delete;
  IInstructionEditorActionHandler& operator=(const IInstructionEditorActionHandler&) = delete;
  IInstructionEditorActionHandler(IInstructionEditorActionHandler&&) = delete;
  IInstructionEditorActionHandler& operator=(IInstructionEditorActionHandler&&) = delete;

  /**
   * @brief Drops instruction on the canvas.
   *
   * The method ignores existing selection and simply appends an instruction to the instruction
   * container.
   *
   * @param item_type The type name of a single instruction or the name of a registered sub-tree.
   * @param pos The position of instruction on graphics canvas.
   */
  virtual void DropInstruction(const std::string& item_type, const position_t& pos) = 0;

  /**
   * @brief Checks if instruction of the given type can be inserted after currently selected
   * instruction.
   */
  virtual bool CanInsertInstructionAfter(const std::string& item_type) const = 0;

  /**
   * @brief Inserts new instruction of given type after the current selection.
   *
   * @param item_type The type name of a single instruction or the name of a registered sub-tree.
   */
  virtual void InsertInstructionAfter(const std::string& item_type) = 0;

  /**
   * @brief Checks if SessionItem of the given type can be inserted into currently selected
   * instruction.
   */
  virtual bool CanInsertInstructionInto(const std::string& item_type) const = 0;

  /**
   * @brief Inserts new instruction of given type into the current selection.
   *
   * @param item_type The type name of a single instruction or the name of a registered sub-tree.
   */
  virtual void InsertInstructionInto(const std::string& item_type) = 0;

  /**
   * @brief Checks if currently selected instruction can be removed.
   */
  virtual bool CanRemoveInstruction() const = 0;

  /**
   * @brief Removes currently selected instruction.
   */
  virtual void RemoveInstruction() = 0;

  /**
   * @brief Moves instruction up (decrement index) in parent's container.
   *
   * @details The method doesn't change a parent, so will do nothing for the first child.
   */
  virtual void MoveUp() = 0;

  /**
   * @brief Moves instruction down (increment index) in parent's container.
   *
   * @details The method doesn't change a parent, so will do nothing for the last child.
   */
  virtual void MoveDown() = 0;

  /**
   * @brief Cals external dialog for AnyValueItem editing.
   */
  virtual void OnEditAnyvalueRequest() = 0;

  /**
   * @brief Checks if cut operation is possible.
   */
  virtual bool CanCut() const = 0;

  /**
   * @brief Cut selected instruction.
   */
  virtual void Cut() = 0;

  /**
   * @brief Checks if copy operation is possible.
   */
  virtual bool CanCopy() const = 0;

  /**
   * @brief Copy selected instruction.
   */
  virtual void Copy() = 0;

  /**
   * @brief Checks if paste-after operation is possible.
   *
   * Paste-after operation inserts new item after the current selection.
   */
  virtual bool CanPasteAfter() const = 0;

  /**
   * @brief Paste new instruction after the current selection.
   */
  virtual void PasteAfter() = 0;

  /**
   * @brief Checks if paste-into operation is possible.
   *
   * Paste-into operation appends a new child to existing children of currently selected
   * instruction.
   */
  virtual bool CanPasteInto() const = 0;

  /**
   * @brief Paste instruction as a child into currently selected instruction.
   *
   * It will be appended to already existing children.
   */
  virtual void PasteInto() = 0;

  /**
   * @brief Inserts given item type in the given parent.
   */
  virtual void InsertItem(const std::string& item_type, mvvm::SessionItem* parent,
                          const mvvm::TagIndex& index) = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_I_INSTRUCTION_EDITOR_ACTION_HANDLER_H_
