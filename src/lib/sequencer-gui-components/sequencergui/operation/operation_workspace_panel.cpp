/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "operation_workspace_panel.h"

#include "operation_workspace_widget.h"

#include <sup/gui/widgets/item_stack_widget.h>
#include <sequencergui/model/procedure_item.h>

#include <QVBoxLayout>

namespace sequencergui
{

OperationWorkspacePanel::OperationWorkspacePanel(QWidget *parent) : QWidget(parent)
      , m_stack_widget(new sup::gui::ItemStackWidget)
      , m_workspace_tree_widget(
          new OperationWorkspaceWidget(OperationWorkspaceWidget::Mode::kWorkspaceTree))
      , m_workspace_table_widget(
          new OperationWorkspaceWidget(OperationWorkspaceWidget::Mode::kWorkspaceTable))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_stack_widget->AddWidget(m_workspace_tree_widget);
  m_stack_widget->AddWidget(m_workspace_table_widget);
  m_stack_widget->SetCurrentIndex(0);

  layout->addWidget(m_stack_widget);
}

OperationWorkspacePanel::~OperationWorkspacePanel() = default;

void OperationWorkspacePanel::SetProcedure(ProcedureItem *item)
{
  m_workspace_tree_widget->SetProcedure(item);
  m_workspace_table_widget->SetProcedure(item);
}

}  // namespace sequencergui
