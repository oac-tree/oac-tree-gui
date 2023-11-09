/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_OPERATION_INSTRUCTION_TREE_SELECTION_CONTROLLER_H_
#define SEQUENCERGUI_OPERATION_INSTRUCTION_TREE_SELECTION_CONTROLLER_H_

#include <QObject>

class QTreeView;

namespace mvvm
{
class ViewModel;
class SessionItem;
}  // namespace mvvm

namespace sequencergui
{

class InstructionItem;
class InstructionContainerItem;

/**
 * @brief The InstructionTreeSelectionController helper class provides logic for instruction
 * selection on real-time instruction tree.
 *
 * As an input, it uses the instruction currenly being executed. If the instruction is located
 * inside collapsed branch, the algorithm will look for the parent which ownes collapsed branch, and
 * will select it instead. If current instruction is inside expanded branch, will simply select it.
 */
class InstructionTreeSelectionController : public QObject
{
  Q_OBJECT

public:
  explicit InstructionTreeSelectionController(QTreeView* tree_view, QObject* parent = nullptr);
  ~InstructionTreeSelectionController() override;

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

  mvvm::SessionItem* FindVisibleInstruction(const mvvm::SessionItem* item) const;

  /**
   * @brief Sets the instruction tree view to the default expand state.
   *
   * @details The default expand state is what follows from is_collapsed property on board of
   * instruction items.
   */
  void SetDefaultExpandState();

private:
  mvvm::ViewModel* GetViewModel() const;

  QTreeView* m_tree_view{nullptr};
  InstructionContainerItem* m_instruction_container{nullptr};

  std::vector<InstructionItem*> m_selection_preferences;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_INSTRUCTION_TREE_SELECTION_CONTROLLER_H_
