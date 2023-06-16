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

#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/all_items_tree_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QToolBar>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

OperationWorkspacePanel::OperationWorkspacePanel(QWidget *parent)
    : QWidget(parent), m_workspace_tree(new mvvm::AllItemsTreeView), m_tool_bar(new QToolBar)
{
  setWindowTitle("Workspace");

  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_workspace_tree);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_tool_bar->setIconSize(styleutils::ToolBarIconSize());

  auto toggle_panel_button = new QToolButton;
  toggle_panel_button->setText("Workspace");
  toggle_panel_button->setIcon(styleutils::GetIcon("menu-open"));
  toggle_panel_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  toggle_panel_button->setToolTip("Show/hide Workspace panel");

  auto on_toggle = [this]() { setVisible(!isVisible()); };
  connect(toggle_panel_button, &QToolButton::clicked, this, on_toggle);

  m_tool_bar->addWidget(toggle_panel_button);
}

OperationWorkspacePanel::~OperationWorkspacePanel() = default;

void OperationWorkspacePanel::SetProcedure(ProcedureItem *procedure_item)
{
  m_workspace_tree->SetItem(procedure_item ? procedure_item->GetWorkspace() : nullptr);
}

QToolBar *OperationWorkspacePanel::GetToolBar() const
{
  return m_tool_bar;
}

}  // namespace sequencergui
