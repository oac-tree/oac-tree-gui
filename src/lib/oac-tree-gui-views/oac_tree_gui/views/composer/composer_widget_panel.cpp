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

#include "composer_widget_panel.h"

#include "instruction_editor_widget.h"
#include "workspace_editor_widget.h"
#include "xml_panel.h"

#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/views/nodeeditor/node_editor_widget.h>

#include <sup/gui/widgets/item_stack_widget.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

namespace
{

QString GetStackWidgetSettingsKey(const QString& settings_group_name)
{
  return settings_group_name + "_stack_widget";
}

}  // namespace

ComposerWidgetPanel::ComposerWidgetPanel(sup::gui::IAppCommandService& command_service,
                                         const QString& settings_group_name, WidgetType widget_type,
                                         QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_instruction_editor_widget(new InstructionEditorWidget(command_service))
    , m_workspace_editor_widget(
          new WorkspaceEditorWidget(command_service, WorkspacePresentationType::kWorkspaceTree))
    , m_node_editor(new NodeEditorWidget)
    , m_xml_panel(new XmlPanel)
    , m_stack_widget(new sup::gui::ItemStackWidget(GetStackWidgetSettingsKey(settings_group_name)))
{
  setWindowTitle("Composer");

  auto layout = new QVBoxLayout(this);

  m_stack_widget->AddWidget(m_instruction_editor_widget, m_instruction_editor_widget->actions());
  m_stack_widget->AddWidget(m_workspace_editor_widget, m_workspace_editor_widget->actions());
  m_stack_widget->AddWidget(m_node_editor, m_node_editor->actions());
  m_stack_widget->AddWidget(m_xml_panel, m_xml_panel->actions());

  layout->addWidget(m_stack_widget);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  SetupConnections();

  m_stack_widget->SetCurrentIndex(static_cast<int>(widget_type));
}

ComposerWidgetPanel::~ComposerWidgetPanel() = default;

void ComposerWidgetPanel::SetProcedure(ProcedureItem* procedure)
{
  m_procedure = procedure;
  m_instruction_editor_widget->SetInstructionContainer(
      m_procedure ? m_procedure->GetInstructionContainer() : nullptr);
  m_workspace_editor_widget->SetWorkspaceItem(m_procedure ? m_procedure->GetWorkspace() : nullptr);
  m_node_editor->SetProcedure(m_procedure);
  m_xml_panel->SetProcedure(m_procedure);
}

void ComposerWidgetPanel::SetSelectedInstructions(const std::vector<InstructionItem*>& instructions)
{
  m_block_selection_notification = true;

  m_instruction_editor_widget->SetSelectedInstructions(instructions);
  m_node_editor->SetSelectedInstructions(instructions);

  m_block_selection_notification = false;
}

std::vector<InstructionItem*> ComposerWidgetPanel::GetSelectedInstructions() const
{
  if (m_stack_widget->GetCurrentWidget() == m_instruction_editor_widget)
  {
    return m_instruction_editor_widget->GetSelectedInstructions();
  }

  if (m_stack_widget->GetCurrentWidget() == m_node_editor)
  {
    return m_node_editor->GetSelectedInstructions();
  }

  return {};
}

InstructionItem* ComposerWidgetPanel::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

void ComposerWidgetPanel::ReadSettings()
{
  m_stack_widget->ReadSettings(sup::gui::GetSettingsReadFunc());
}

void ComposerWidgetPanel::WriteSettings()
{
  m_stack_widget->WriteSettings(sup::gui::GetSettingsWriteFunc());
}

void ComposerWidgetPanel::SetupConnections()
{
  auto on_scene_instruction_selected = [this]()
  {
    if (!m_block_selection_notification)
    {
      m_block_selection_notification = true;
      auto selected = m_node_editor->GetSelectedInstructions();
      m_instruction_editor_widget->SetSelectedInstructions(selected);
      emit InstructionSelected(selected.empty() ? nullptr : selected.front());
      m_block_selection_notification = false;
    }
  };
  connect(m_node_editor, &NodeEditorWidget::selectionChanged, this, on_scene_instruction_selected);

  auto on_tree_instruction_selected = [this](auto instruction)
  {
    if (!m_block_selection_notification)
    {
      m_block_selection_notification = true;
      m_node_editor->SetSelectedInstructions(
          m_instruction_editor_widget->GetSelectedInstructions());
      emit InstructionSelected(instruction);
      m_block_selection_notification = false;
    }
  };
  connect(m_instruction_editor_widget, &InstructionEditorWidget::InstructionSelected, this,
          on_tree_instruction_selected);

  connect(m_xml_panel, &XmlPanel::ExportToFileRequest, this,
          &ComposerWidgetPanel::ExportToFileRequest);
}

}  // namespace oac_tree_gui
