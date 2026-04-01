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

#include "task_widget.h"

#include "status_indicator_widget.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

namespace oac_tree_gui
{

TaskWidget::TaskWidget(const QString& instruction_name, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_status_widget(new StatusIndicatorWidget(this))
    , m_instruction_label(new QLabel(instruction_name))
    , m_children_group(new QGroupBox)
{
  // children group box
  auto children_group_layout = new QVBoxLayout;
  m_children_group->setLayout(children_group_layout);

  // layout with label and children group
  auto vertical_layout = new QVBoxLayout;
  vertical_layout->addWidget(m_instruction_label);
  vertical_layout->addWidget(m_children_group);
  m_children_group->setVisible(false);

  // main layout
  auto layout = new QHBoxLayout(this);
  layout->addWidget(m_status_widget);
  layout->addLayout(vertical_layout);
}

InstructionStatus TaskWidget::GetInstructionStatus() const
{
  return m_status_widget->GetInstructionStatus();
}

void TaskWidget::SetInstructionStatus(InstructionStatus status)
{
  m_status_widget->SetInstructionStatus(status);
}

TaskWidget* TaskWidget::CreateAndAddChild(const QString& child_name)
{
  auto child_widget = new TaskWidget(child_name, this);
  m_children_widgets.push_back(child_widget);
  m_children_group->layout()->addWidget(child_widget);
  m_children_group->setVisible(true);
  m_status_widget->SetVerticalExpandPolicy(true);
  return child_widget;
}

void TaskWidget::AddSeparator()
{
  auto frame = new QFrame;
  frame->setFrameShape(QFrame::HLine);
  frame->setFrameShadow(QFrame::Sunken);
  m_children_group->layout()->addWidget(frame);
}

QString TaskWidget::GetLabelText() const
{
  return m_instruction_label->text();
}

std::vector<TaskWidget*> TaskWidget::GetChildWidgets() const
{
  return m_children_widgets;
}

}  // namespace oac_tree_gui
