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

InstructionTaskAreaWidget::InstructionTaskAreaWidget(QWidget* parent_widget)
    : QWidget(parent_widget), m_scroll_area(new QScrollArea)
{
  m_scroll_area->setWidgetResizable(true);
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_scroll_area);
}

void InstructionTaskAreaWidget::SetTaskWidget(TaskWidget* task_widget)
{
  if (m_scroll_area->widget())
  {
    m_scroll_area->widget()->deleteLater();
  }
  m_scroll_area->setWidget(CreateWrapperWidget(task_widget).release());
}

std::unique_ptr<QWidget> InstructionTaskAreaWidget::CreateWrapperWidget(
    TaskWidget* task_widget) const
{
  auto wrapper_widget = std::make_unique<QWidget>();
  auto layout = new QVBoxLayout(wrapper_widget.get());
  layout->addWidget(task_widget);
  layout->addStretch();
  return wrapper_widget;
}

}  // namespace oac_tree_gui
