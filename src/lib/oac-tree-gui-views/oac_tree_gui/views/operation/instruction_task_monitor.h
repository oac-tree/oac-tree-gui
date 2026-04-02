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

#include <mvvm/signals/event_types.h>

#include <QWidget>
#include <memory>

namespace mvvm
{
class ModelListener;
}

namespace oac_tree_gui
{

class InstructionTaskAreaWidget;
class TaskWidget;
class InstructionContainerItem;
class InstructionTaskWidgetBuilder;

/**
 * @brief The InstructionTaskAreaWidget class is a scroll area holding TaskWidget with the
 * instruction task tree.
 */
class InstructionTaskMonitor : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionTaskMonitor(QWidget* parent_widget = nullptr);
  ~InstructionTaskMonitor() override;

  InstructionTaskMonitor(const InstructionTaskMonitor&) = delete;
  InstructionTaskMonitor& operator=(const InstructionTaskMonitor&) = delete;
  InstructionTaskMonitor(InstructionTaskMonitor&&) noexcept = delete;
  InstructionTaskMonitor& operator=(InstructionTaskMonitor&&) noexcept = delete;

  void SetInstructionContainer(InstructionContainerItem* container);

private:
  void OnDataChangedEvent(const mvvm::DataChangedEvent& event);

  InstructionTaskAreaWidget* m_task_area_widget{nullptr};
  std::unique_ptr<InstructionTaskWidgetBuilder> m_task_widget_builder;
  std::unique_ptr<mvvm::ModelListener> m_listener;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_MONITOR_H_
