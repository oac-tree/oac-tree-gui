/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "procedure_composer_tab_widget.h"

#include "instruction_editor_widget.h"
#include "workspace_editor_widget.h"
#include "xml_panel.h"

#include <oac_tree_gui/views/nodeeditor/node_editor_widget.h>

#include <sup/gui/style/style_helper.h>

#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>

namespace oac_tree_gui
{

ProcedureComposerTabWidget::ProcedureComposerTabWidget(
    sup::gui::IAppCommandService& command_service, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new QToolBar)
    , m_instruction_editor_widget(new InstructionEditorWidget(command_service))
    , m_workspace_editor_widget(
          new WorkspaceEditorWidget(command_service, WorkspacePresentationType::kWorkspaceTree))
    , m_node_editor(new NodeEditorWidget(NodeEditorMode::kNodeEditor))
    , m_xml_panel(new XmlPanel)
    , m_tab_widget(new QTabWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_tool_bar->setIconSize(sup::gui::utils::NarrowToolBarIconSize());
  m_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tab_widget);

  m_tab_widget->addTab(m_instruction_editor_widget, "Instruction Tree");
  m_tab_widget->addTab(m_workspace_editor_widget, "Workspace");
  m_tab_widget->addTab(m_node_editor, "Node editor");
  m_tab_widget->addTab(m_xml_panel, "XML");
  m_tab_widget->setTabPosition(QTabWidget::South);

  connect(m_tab_widget, &QTabWidget::currentChanged, this, [this](int) { UpdateToolbarButtons(); });
  UpdateToolbarButtons();
}

ProcedureComposerTabWidget::~ProcedureComposerTabWidget() = default;

void ProcedureComposerTabWidget::SetProcedure(ProcedureItem* procedure_item)
{
  (void)procedure_item;
  m_instruction_editor_widget->SetInstructionContainer(
      procedure_item ? procedure_item->GetInstructionContainer() : nullptr);
  m_workspace_editor_widget->SetWorkspaceItem(procedure_item ? procedure_item->GetWorkspace()
                                                             : nullptr);
  m_node_editor->SetProcedure(procedure_item);
  m_xml_panel->SetProcedure(procedure_item);
}

ProcedureEditorType ProcedureComposerTabWidget::GetEditorType() const
{
  return static_cast<ProcedureEditorType>(m_tab_widget->currentIndex());
}

void ProcedureComposerTabWidget::SetEditorType(ProcedureEditorType edit_type)
{
  m_tab_widget->setCurrentIndex(static_cast<std::int32_t>(edit_type));
}

void ProcedureComposerTabWidget::UpdateToolbarButtons()
{
  m_tool_bar->clear();
  m_tool_bar->addActions(m_tab_widget->currentWidget()->actions());
}

}  // namespace oac_tree_gui
