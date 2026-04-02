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

void InstructionTaskAreaWidget::SetTaskWidget(std::unique_ptr<TaskWidget> task_widget)
{
  Clear();
  AddWidgetToArea(std::move(task_widget));
}

void InstructionTaskAreaWidget::Clear()
{
  if (m_scroll_area->widget() != nullptr)
  {
    m_scroll_area->takeWidget()->deleteLater();
  }
}

void InstructionTaskAreaWidget::AddWidgetToArea(std::unique_ptr<TaskWidget> task_widget) const
{
  // we need wrapper widget to add stretch at the end of task widget to align it to top of scroll
  // area
  auto wrapper_widget = new QWidget;
  auto layout = new QVBoxLayout(wrapper_widget);
  layout->addWidget(task_widget.release());  // task widget ownership goes to layout
  layout->addStretch();
  m_scroll_area->setWidget(wrapper_widget);  // wrapper ownership goes to scroll area
}

}  // namespace oac_tree_gui
