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
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/signals/model_listener.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

InstructionTaskMonitor::InstructionTaskMonitor(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_task_area_widget(new InstructionTaskAreaWidget(this))
    , m_task_widget_builder(std::make_unique<InstructionTaskWidgetBuilder>())
{
  setWindowTitle("Instruction Task Monitor");

  auto layout = new QVBoxLayout(this);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_task_area_widget);
}

InstructionTaskMonitor::~InstructionTaskMonitor() = default;

void InstructionTaskMonitor::SetInstructionContainer(InstructionContainerItem* container)
{
  if (container == nullptr)
  {
    m_task_area_widget->Clear();
    m_task_widget_builder.reset();
    return;
  }

  m_task_widget_builder = std::make_unique<InstructionTaskWidgetBuilder>();
  m_listener = std::make_unique<mvvm::ModelListener>(container->GetModel());
  m_listener->Connect<mvvm::DataChangedEvent>(this, &InstructionTaskMonitor::OnDataChangedEvent);

  for (auto instruction : container->GetInstructions())
  {
    // we create only first widget for the instruction task. In any case real time container can
    // have only one instruction, so it should be enough.
    m_task_area_widget->SetTaskWidget(m_task_widget_builder->CreateTaskWidget(*instruction));
    break;
  }
}

const InstructionTaskWidgetBuilder* InstructionTaskMonitor::GetTaskWidgetBuilder() const
{
  return m_task_widget_builder.get();
}

void InstructionTaskMonitor::OnDataChangedEvent(const mvvm::DataChangedEvent& event)
{
  auto instruction_item = mvvm::utils::FindItemUp<InstructionItem>(event.item);
  if (instruction_item != nullptr)
  {
    if (auto task_widget = m_task_widget_builder->FindWidgetForInstruction(instruction_item);
        task_widget)
    {
      task_widget->SetInstructionStatus(instruction_item->GetStatus());
    }
  }
}

}  // namespace oac_tree_gui
