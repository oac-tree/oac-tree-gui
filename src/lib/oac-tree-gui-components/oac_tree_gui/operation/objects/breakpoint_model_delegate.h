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

#ifndef OAC_TREE_GUI_OPERATION_OBJECTS_BREAKPOINT_MODEL_DELEGATE_H_
#define OAC_TREE_GUI_OPERATION_OBJECTS_BREAKPOINT_MODEL_DELEGATE_H_

#include <QStyledItemDelegate>

namespace oac_tree_gui
{

/**
 * @brief The BreakpointModelDelegate class is a delagate for InstructionViewModel with the task to
 * paint breakpoints in a 3rd row.
 */
class BreakpointModelDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  explicit BreakpointModelDelegate(QObject* parent = nullptr);

  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_OPERATION_OBJECTS_BREAKPOINT_MODEL_DELEGATE_H_
