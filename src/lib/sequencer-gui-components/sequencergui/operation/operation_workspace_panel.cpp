/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sequencergui/model/procedure_item.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <QSettings>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("OperationWorkspacePanel");
const QString kWorkspaceIndexSettingName = kGroupName + "/" + "workspace_index";
}  // namespace

namespace sequencergui
{

OperationWorkspacePanel::OperationWorkspacePanel(QWidget *parent)
    : QWidget(parent)
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

  layout->addWidget(m_stack_widget);

  ReadSettings();
}

OperationWorkspacePanel::~OperationWorkspacePanel()
{
  WriteSettings();
}

void OperationWorkspacePanel::SetProcedure(ProcedureItem *item)
{
  m_workspace_tree_widget->SetProcedure(item);
  m_workspace_table_widget->SetProcedure(item);
}

void OperationWorkspacePanel::ReadSettings()
{
  const QSettings settings;
  auto current_index = settings.value(kWorkspaceIndexSettingName, 0).toInt();
  m_stack_widget->SetCurrentIndex(current_index);
}

void OperationWorkspacePanel::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWorkspaceIndexSettingName, m_stack_widget->GetCurrentIndex());
}

}  // namespace sequencergui
