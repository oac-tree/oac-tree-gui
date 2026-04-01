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

#include "instruction_task_area_widget.h"

#include "task_widget.h"

#include <QScrollArea>
#include <QVBoxLayout>

namespace oac_tree_gui
{

InstructionTaskAreaWidget::InstructionTaskAreaWidget(TaskWidget* root_task_widget,
                                                     QWidget* parent_widget)
    : QWidget(parent_widget), m_scroll_area(new QScrollArea)
{
  m_scroll_area->setWidgetResizable(true);

  auto scroll_area_widget = new QWidget;
  auto scroll_layout = new QVBoxLayout(scroll_area_widget);
  scroll_layout->addWidget(root_task_widget);
  scroll_layout->addStretch();
  m_scroll_area->setWidget(scroll_area_widget);

  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_scroll_area);
}

}  // namespace oac_tree_gui
