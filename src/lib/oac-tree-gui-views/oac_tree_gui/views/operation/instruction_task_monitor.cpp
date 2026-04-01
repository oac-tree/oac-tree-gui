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

#include "instruction_task_monitor.h"

#include "instruction_task_area_widget.h"
#include "instruction_task_widget_builder.h"
#include "task_widget.h"

#include <oac_tree_gui/model/instruction_container_item.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

InstructionTaskMonitor::InstructionTaskMonitor(QWidget* parent_widget)
    : QWidget(parent_widget), m_task_area_widget(new InstructionTaskAreaWidget(this))
{
  auto layout = new QVBoxLayout(this);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_task_area_widget);

  // m_task_area_widget->SetTaskWidget(CreateTestTask().release());
}

void InstructionTaskMonitor::SetInstructionContainer(InstructionContainerItem* container)
{
  InstructionTaskWidgetBuilder builder;

  for (auto instruction : container->GetInstructions())
  {
    // we create only first widget for the instruction task. In any case real time container can have
    // only one instruction, so it should be enough.
    if (auto task_widget = builder.CreateTaskWidget(*instruction))
    {
      m_task_area_widget->SetTaskWidget(task_widget.release());
      break;
    }
  }
}

std::unique_ptr<TaskWidget> InstructionTaskMonitor::CreateTestTask() const
{
  auto result = std::make_unique<TaskWidget>("Sequence");
  result->SetInstructionStatus(InstructionStatus::kFailure);

  auto child0 = result->CreateAndAddChild("wait0");
  child0->SetInstructionStatus(InstructionStatus::kSuccess);

  result->AddSeparator();
  auto child1 = result->CreateAndAddChild("wait1");
  child1->SetInstructionStatus(InstructionStatus::kRunning);

  result->AddSeparator();
  auto sequence = result->CreateAndAddChild("Sequence");
  auto child2 = sequence->CreateAndAddChild("wait2");
  child2->SetInstructionStatus(InstructionStatus::kNotFinished);
  sequence->AddSeparator();
  auto child3 = sequence->CreateAndAddChild("wait3");
  child3->SetInstructionStatus(InstructionStatus::kNotFinished);

  return result;
}

}  // namespace oac_tree_gui
