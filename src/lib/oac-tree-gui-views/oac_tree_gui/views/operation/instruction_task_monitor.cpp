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
#include "task_widget.h"

#include <QVBoxLayout>

namespace oac_tree_gui
{

InstructionTaskMonitor::InstructionTaskMonitor(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_task_area_widget(new InstructionTaskAreaWidget(CreateTestTask().release()))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_task_area_widget);
}

std::unique_ptr<TaskWidget> InstructionTaskMonitor::CreateTestTask() const
{
  auto result = std::make_unique<TaskWidget>("Sequence");
  result->SetInstructionStatus(InstructionStatus::kFailure);

  auto wait0 = result->CreateAndAddChild("wait0");
  wait0->SetInstructionStatus(InstructionStatus::kSuccess);
  auto wait1 = result->CreateAndAddChild("wait1");
  wait1->SetInstructionStatus(InstructionStatus::kRunning);

  return result;
}

}  // namespace oac_tree_gui
