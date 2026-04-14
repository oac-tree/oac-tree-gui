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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_AREA_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_AREA_WIDGET_H_

#include <QWidget>

class QScrollArea;

namespace oac_tree_gui
{

class TaskWidget;

/**
 * @brief The InstructionTaskAreaWidget class is a scroll area holding TaskWidget with the
 * instruction task tree.
 */
class InstructionTaskAreaWidget : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionTaskAreaWidget(QWidget* parent_widget = nullptr);

  /**
   * @brief Set widget to show in a scroll area.
   *
   * Ownership of the widget is transferred to InstructionTaskAreaWidget, so it will be deleted when
   * the new widget is set or when InstructionTaskAreaWidget is deleted.
   */
  void SetTaskWidget(std::unique_ptr<TaskWidget> task_widget);

  /**
   * @brief Clear the content of scroll area.
   *
   * All TaskWidgets will be deleted.
   */
  void Clear();

private:
  void AddWidgetToArea(std::unique_ptr<TaskWidget> task_widget) const;

  QScrollArea* m_scroll_area{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_INSTRUCTION_TASK_AREA_WIDGET_H_
