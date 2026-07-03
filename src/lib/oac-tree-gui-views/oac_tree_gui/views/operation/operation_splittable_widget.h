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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_SPLITTABLE_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_SPLITTABLE_WIDGET_H_

#include <QWidget>

namespace oac_tree_gui
{

//! Placeholder widget for future operation splittable development.

class OperationSplittableWidget : public QWidget
{
  Q_OBJECT

public:
  explicit OperationSplittableWidget(QWidget* parent_widget = nullptr);
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_SPLITTABLE_WIDGET_H_
