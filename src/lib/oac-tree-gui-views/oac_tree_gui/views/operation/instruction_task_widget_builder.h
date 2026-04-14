/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_WIDGET_BUILDER_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_WIDGET_BUILDER_H_

#include <map>
#include <memory>

namespace oac_tree_gui
{

class TaskWidget;
class InstructionItem;

/**
 * @brief The InstructionTaskWidgetBuilder class is used to build nested
 * TaskWidget structure representing given root instruction.
 *
 * It also allows to find particular TaskWIdget for a given child instruction.
 */
class InstructionTaskWidgetBuilder
{
public:
  InstructionTaskWidgetBuilder() = default;

  std::unique_ptr<TaskWidget> CreateTaskWidget(const InstructionItem& root_instruction);

  /**
   * @brief Finds TaskWidget representing given instruction.
   */
  TaskWidget* FindWidgetForInstruction(const InstructionItem* item);

  /**
   * @brief Returns number of instruction-widget pairs stored in the builder.
   */
  std::size_t GetInstructionCount() const;

private:
  std::map<const InstructionItem*, TaskWidget*> m_instruction_to_widget;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_WIDGET_BUILDER_H_
