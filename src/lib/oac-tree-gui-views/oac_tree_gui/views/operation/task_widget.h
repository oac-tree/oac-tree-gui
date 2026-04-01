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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_TASK_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_TASK_WIDGET_H_

#include <oac_tree_gui/model/instruction_status.h>

#include <QWidget>

class QLabel;
class QGroupBox;

namespace oac_tree_gui
{

class StatusIndicatorWidget;

/**
 * @brief Main widget representing running instruction in a task tree.
 *
 * Contains a vertical colored bar representing execution status, a label with long description, and
 * optional group box for children's widget.
 */
class TaskWidget : public QWidget
{
  Q_OBJECT

public:
  explicit TaskWidget(const QString& instruction_name, QWidget* parent_widget = nullptr);

  /**
   * @brief Returns current instruction status shown by the widget.
   */
  InstructionStatus GetInstructionStatus() const;

  /**
   * @brief Update indicator color to match instruction execution status.
   */
  void SetInstructionStatus(InstructionStatus status);

  /**
   * @brief Creates and adds child widget to the group box. The group box is made visible if it was
   * hidden before.
   */
  TaskWidget* CreateAndAddChild(const QString& child_name);

  /**
   * @brief Adds separator line between children widgets.
   */
  void AddSeparator();

  /**
   * @brief Returns text of the instruction label.
   */
  QString GetLabelText() const;

  /**
   * @brief Returns vector of child TaskWidgets.
   */
  std::vector<TaskWidget*> GetChildWidgets() const;

private:
  StatusIndicatorWidget* m_status_widget{nullptr};
  QLabel* m_instruction_label{nullptr};
  QGroupBox* m_children_group{nullptr};
  std::vector<TaskWidget*> m_children_widgets;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_TASK_WIDGET_H_
