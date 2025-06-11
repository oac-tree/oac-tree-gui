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

#ifndef OAC_TREE_GUI_OPERATION_INSTRUCTION_TREE_EXPAND_CONTROLLER_H_
#define OAC_TREE_GUI_OPERATION_INSTRUCTION_TREE_EXPAND_CONTROLLER_H_

#include <QObject>

class QTreeView;

namespace mvvm
{
class ViewModel;
class SessionItem;
}  // namespace mvvm

namespace oac_tree_gui
{

class InstructionItem;
class InstructionContainerItem;

/**
 * @brief The InstructionTreeExpandController helper class provides a logic for instruction
 * highlight depending on the tree branch collapse/expand status.
 *
 * @details It allows to save desired instruction list to highlight in a buffer, and then retrieve
 * an actual list to highlight at the later stage, for current tree branch collapse/expand status.
 *
 * As an input, it uses instructions currenly being executed. If the instruction is located
 * inside collapsed branch, the algorithm will look for the parent which owns collapsed branch, and
 * will select it instead. If current instruction is inside expanded branch, will simply select it.
 */
class InstructionTreeExpandController : public QObject
{
  Q_OBJECT

public:
  explicit InstructionTreeExpandController(QTreeView* tree_view, QObject* parent_object = nullptr);
  ~InstructionTreeExpandController() override;

  void SetInstructionContainer(InstructionContainerItem* instruction_container);

  /**
   * @brief Save list of instructions to select.
   *
   * @details The actual list of instruction to select can be obtained via GetInstructionToSelect
   * and will depend on current branch collapse/expand status.
   */
  void SaveSelectionRequest(const std::vector<InstructionItem*>& instructions);

  /**
   * @brief Returns list of instructions to make actual selection.
   *
   * @details Will take into account branch collapse/expand status and transform selection
   * preferences to the real list of instructions to select. It is expected that
   * SaveSelectionRequest has beem called before.
   */
  std::vector<mvvm::SessionItem*> GetInstructionsToSelect() const;

  /**
   * @brief Finds visible instruction up in the hierarchy located in non-collapsed branch.
   *
   * @details If all branches are in expand state, will simply return same item back.
   */
  mvvm::SessionItem* FindVisibleInstruction(const mvvm::SessionItem* item) const;

  /**
   * @brief Sets the QTreeView to the expand state of instructions.
   */
  void SetTreeViewToInstructionExpandState();

signals:
  void VisibilityHasChanged();

private:
  mvvm::ViewModel* GetViewModel() const;
  void OnTreeCollapsedChange(const QModelIndex& index);
  InstructionItem* GetInstruction(const QModelIndex& index);

  QTreeView* m_tree_view{nullptr};
  InstructionContainerItem* m_instruction_container{nullptr};

  std::vector<InstructionItem*> m_selection_preferences;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_OPERATION_INSTRUCTION_TREE_EXPAND_CONTROLLER_H_
