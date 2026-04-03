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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_STATUS_INDICATOR_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_STATUS_INDICATOR_H_

#include <oac_tree_gui/model/instruction_status.h>

#include <QLabel>

namespace oac_tree_gui
{

class ProgressIndicator;

/**
 * @brief The StatusIndicator class is a vertical colored bar representing InstructionItem execution
 * status.
 *
 * It is implemented as a QLabel since it is a simple way to have color and text together.
 */
class StatusIndicator : public QLabel
{
  Q_OBJECT

public:
  explicit StatusIndicator(QWidget* parent_widget = nullptr);

  /**
   * @brief Update indicator color to match instruction execution status.
   */
  void SetInstructionStatus(InstructionStatus status);

  /**
   * @brief Returns current instruction status shown by the widget.
   */
  InstructionStatus GetInstructionStatus() const;

private:
  void UpdateAppearance();
  InstructionStatus m_current_status{InstructionStatus::kUndefined};
  ProgressIndicator* m_progress_indicator{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_STATUS_INDICATOR_H_
