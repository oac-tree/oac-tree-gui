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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_STATUS_INDICATOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_STATUS_INDICATOR_WIDGET_H_

#include <oac_tree_gui/model/instruction_status.h>

#include <QWidget>

namespace oac_tree_gui
{

class StatusIndicator;

/**
 * @brief The vertical colored bar representing InstructionItem execution status.
 */
class StatusIndicatorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit StatusIndicatorWidget(QWidget* parent_widget = nullptr);

  /**
   * @brief Update indicator color to match instruction execution status.
   */
  void SetInstructionStatus(InstructionStatus status);

  /**
   * @brief Returns current instruction status shown by the widget.
   */
  InstructionStatus GetInstructionStatus() const;

  /**
   * @brief Will be shown as long vertical bar if true, and as small square if false.
   *
   * Used for different appearance of branches and leafs in the task tree.
   */
  void SetVerticalExpandPolicy(bool expand);

private:
  StatusIndicator* m_status_indicator{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_STATUS_INDICATOR_WIDGET_H_
