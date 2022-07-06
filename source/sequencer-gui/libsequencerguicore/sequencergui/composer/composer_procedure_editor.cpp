/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/composer/composer_procedure_editor.h"

#include <mvvm/widgets/property_tree_view.h>
#include <mvvm/widgets/widget_utils.h>
#include <sequencergui/composer/composer_actions.h>
#include <sequencergui/composer/instruction_tree_widget.h>
#include <sequencergui/composer/workspace_list_widget.h>
#include <sequencergui/core/message_handler_interface.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_variable_items.h>

#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QToolBar>

namespace sequencergui
{
ComposerProcedureEditor::ComposerProcedureEditor(
    std::unique_ptr<MessageHandlerInterface> message_handler, QWidget* parent)
    : QWidget(parent)
    , m_tool_bar(new QToolBar)
    , m_tab_widget(new QTabWidget)
    , m_instruction_tree(new InstructionTreeWidget)
    , m_workspace_tree(new WorkspaceListWidget)
    , m_property_tree(new mvvm::PropertyTreeView)
    , m_splitter(new QSplitter)
    , m_composer_actions(std::make_unique<ComposerActions>())
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  m_splitter->setOrientation(Qt::Vertical);

  m_tab_widget->addTab(m_instruction_tree, "Instructions");
  m_tab_widget->addTab(m_workspace_tree, "Workspace");
  m_tab_widget->setTabPosition(QTabWidget::South);
  m_splitter->addWidget(m_tab_widget);
  m_splitter->addWidget(m_property_tree);
  m_splitter->setSizes(QList<int>() << 300 << 200);
  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);

  // setting up ComposerActions
  m_composer_actions->SetMessageHandler(std::move(message_handler));
  m_composer_actions->SetContext(CreateComposerContext());

  SetToolBarWidgets(m_instruction_tree->GetToolBarWidgets());

  auto on_tabbar_changed = [this](int index)
  {
    // FIXME simplify
    if (index == 0)
    {
      SetToolBarWidgets(m_instruction_tree->GetToolBarWidgets());
    }
    else
    {
      SetToolBarWidgets(m_workspace_tree->GetToolBarWidgets());
    }
  };
  connect(m_tab_widget, &QTabWidget::currentChanged, this, on_tabbar_changed);

  SetupConnections();
}

ComposerProcedureEditor::~ComposerProcedureEditor() = default;

void ComposerProcedureEditor::SetModel(SequencerModel* model)
{
  m_model = model;
  m_composer_actions->SetModel(model);
}

void ComposerProcedureEditor::SetProcedure(ProcedureItem* procedure)
{
  m_procedure = procedure;
  m_instruction_tree->SetProcedure(m_procedure);
  m_workspace_tree->SetProcedure(m_procedure);
}

void ComposerProcedureEditor::SetSelectedInstructions(
    const std::vector<InstructionItem*>& instructions)
{
  m_instruction_tree->SetSelectedInstructions(instructions);
}

std::vector<InstructionItem*> ComposerProcedureEditor::GetSelectedInstructions() const
{
  return m_instruction_tree->GetSelectedInstructions();
}

InstructionItem* ComposerProcedureEditor::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

void ComposerProcedureEditor::SetToolBarWidgets(const QList<QWidget*>& widgets)
{
  m_tool_bar->clear();
  for (auto widget : widgets)
  {
    m_tool_bar->addWidget(widget);
  }
}

void ComposerProcedureEditor::SetupConnections()
{
  auto on_selection_changed = [this](auto item)
  {
    m_property_tree->SetItem(item);
    emit InstructionSelected(GetSelectedInstruction());
  };
  connect(m_instruction_tree, &InstructionTreeWidget::InstructionSelected, on_selection_changed);

  // propagate instruction related operations from InstructionTreeWidget to ComposerActions
  connect(m_instruction_tree, &InstructionTreeWidget::InsertAfterRequest, m_composer_actions.get(),
          &ComposerActions::OnInsertInstructionAfterRequest);
  connect(m_instruction_tree, &InstructionTreeWidget::InsertIntoRequest, m_composer_actions.get(),
          &ComposerActions::OnInsertInstructionIntoRequest);
  connect(m_instruction_tree, &InstructionTreeWidget::RemoveSelectedRequest,
          m_composer_actions.get(), &ComposerActions::OnRemoveInstructionRequest);

  // propagate variable related operations from WorkspaceListWidget to ComposerActions
  connect(m_workspace_tree, &WorkspaceListWidget::VariableSelected, m_property_tree,
          &::mvvm::PropertyTreeView::SetItem);
  connect(m_workspace_tree, &WorkspaceListWidget::InsertAfterRequest, m_composer_actions.get(),
          &ComposerActions::OnInsertVariableAfterRequest);
  connect(m_workspace_tree, &WorkspaceListWidget::RemoveSelectedRequest, m_composer_actions.get(),
          &ComposerActions::OnRemoveVariableRequest);
}

//! Create context to access current selections performed by the user.
ComposerContext ComposerProcedureEditor::CreateComposerContext()
{
  ComposerContext context;
  context.selected_procedure = [this]() { return m_procedure; };
  context.selected_instruction = [this]() { return m_instruction_tree->GetSelectedInstruction(); };
  context.selected_variable = [this]() { return m_workspace_tree->GetSelectedVariable(); };
  return context;
}

}  // namespace sequencergui
