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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_MONITOR_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_MONITOR_H_

#include <QWidget>

namespace oac_tree_gui
{

class InstructionTaskAreaWidget;
class TaskWidget;
class InstructionContainerItem;

/**
 * @brief The InstructionTaskAreaWidget class is a scroll area holding TaskWidget with the
 * instruction task tree.
 */
class InstructionTaskMonitor : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionTaskMonitor(QWidget* parent_widget = nullptr);

  void SetInstructionContainer(InstructionContainerItem* container);

private:
  std::unique_ptr<TaskWidget> CreateTestTask() const;

  InstructionTaskAreaWidget* m_task_area_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_MONITOR_H_
