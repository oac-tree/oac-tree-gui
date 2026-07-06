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

#include "operation_realtime_widget.h"

#include "message_panel.h"
#include "realtime_instruction_tree_widget.h"

#include <oac_tree_gui/model/instruction_item.h>

#include <sup/gui/widgets/collapsible_list_view.h>

#include <QVBoxLayout>

namespace
{

const QString kCollapsibleListSettingName = "OperationRealTimeWidget/collapsible_list";

}  // namespace

namespace oac_tree_gui
{

OperationRealTimeWidget::OperationRealTimeWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_collapsible_list(new sup::gui::CollapsibleListView(kCollapsibleListSettingName))
    , m_realtime_instruction_tree(new RealTimeInstructionTreeWidget)
    , m_message_panel(new MessagePanel)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_collapsible_list->AddWidget(m_realtime_instruction_tree);

  m_collapsible_list->AddCollapsibleWidget(m_message_panel, m_message_panel->actions());

  layout->addWidget(m_collapsible_list);

  connect(m_realtime_instruction_tree, &RealTimeInstructionTreeWidget::ToggleBreakpointRequest,
          this, &OperationRealTimeWidget::ToggleBreakpointRequest);

  ReadSettings();
}

OperationRealTimeWidget::~OperationRealTimeWidget()
{
  WriteSettings();
}

void OperationRealTimeWidget::SetProcedure(ProcedureItem* procedure_item)
{
  m_realtime_instruction_tree->SetProcedure(procedure_item);
}

void OperationRealTimeWidget::SetSelectedInstructions(
    const std::vector<const InstructionItem*>& items)
{
  m_realtime_instruction_tree->SetSelectedInstructions(items);
}

void OperationRealTimeWidget::SetJobLog(JobLog* job_log)
{
  m_message_panel->SetLog(job_log);
}

void OperationRealTimeWidget::SetViewportFollowsSelectionFlag(bool value)
{
  m_realtime_instruction_tree->SetViewportFollowsSelectionFlag(value);
}

void OperationRealTimeWidget::ReadSettings()
{
  m_collapsible_list->ReadSettings();
}

void OperationRealTimeWidget::WriteSettings()
{
  m_collapsible_list->WriteSettings();
}

}  // namespace oac_tree_gui
