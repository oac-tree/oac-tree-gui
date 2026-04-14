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

#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/signals/model_listener.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

InstructionTaskMonitor::InstructionTaskMonitor(QWidget* parent_widget)
    : QWidget(parent_widget), m_task_area_widget(new InstructionTaskAreaWidget(this))
{
  setWindowTitle("Instruction Task Monitor");

  auto layout = new QVBoxLayout(this);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_task_area_widget);

  SetupVisibilityAgent();
}

InstructionTaskMonitor::~InstructionTaskMonitor()
{
  // we should reset visibility agent since on destruction widget becomes invisible (in
  // uncontrollable moment of time) and agent might try to call invalid callbacks.
  m_visibility_agent.reset();
}

void InstructionTaskMonitor::SetInstructionContainer(InstructionContainerItem* container)
{
  if (container == m_container_item)
  {
    return;
  }

  m_container_item = container;

  if ((m_container_item != nullptr) && isVisible())
  {
    SetInstructionContainerIntern(m_container_item);
  }
}

const InstructionTaskWidgetBuilder* InstructionTaskMonitor::GetTaskWidgetBuilder() const
{
  return m_task_widget_builder.get();
}

void InstructionTaskMonitor::SetInstructionContainerIntern(InstructionContainerItem* container)
{
  if (container == nullptr)
  {
    m_listener.reset();
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

void InstructionTaskMonitor::SetupVisibilityAgent()
{
  auto on_subscribe = [this]() { SetInstructionContainerIntern(m_container_item); };
  auto on_unsubscribe = [this]() { SetInstructionContainerIntern(nullptr); };
  m_visibility_agent =
      std::make_unique<sup::gui::VisibilityAgentBase>(on_subscribe, on_unsubscribe);
  m_visibility_agent->SetTarget(this);
}

}  // namespace oac_tree_gui
