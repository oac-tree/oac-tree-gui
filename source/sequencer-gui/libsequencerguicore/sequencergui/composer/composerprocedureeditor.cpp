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

#include "sequencergui/composer/composerprocedureeditor.h"

#include "sequencergui/composer/composeractions.h"
#include "sequencergui/composer/composertreetoolbar.h"
#include "sequencergui/composer/instructiontreewidget.h"
#include "sequencergui/core/messagehandlerfactory.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/widgets/allitemstreeview.h"
#include "mvvm/widgets/propertytreeview.h"
#include "mvvm/widgets/topitemstreeview.h"
#include "mvvm/widgets/widgetutils.h"

#include <QSplitter>
#include <QTabWidget>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{
ComposerProcedureEditor::ComposerProcedureEditor(QWidget* parent)
    : QWidget(parent)
    , m_tool_bar(new ComposerTreeToolBar)
    , m_tab_widget(new QTabWidget)
    , m_instruction_tree(new InstructionTreeWidget)
    , m_workspace_tree(new mvvm::AllItemsTreeView)
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
  m_composer_actions->SetMessageHandler(CreateStdMessageHandler());
  ComposerContext context;
  context.selected_procedure = [this]() { return m_procedure; };
  context.selected_instruction = [this]() { return m_instruction_tree->GetSelectedInstruction(); };
  m_composer_actions->SetContext(context);

  m_tool_bar->SetWidgets(m_instruction_tree->GetToolBarWidgets());

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
  m_workspace_tree->SetItem(m_procedure ? m_procedure->GetWorkspace() : nullptr);
}

void ComposerProcedureEditor::SetSelectedInstruction(InstructionItem* instruction)
{
  m_instruction_tree->SetSelectedInstruction(instruction);
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

void ComposerProcedureEditor::SetupConnections()
{
  auto on_selection_changed = [this](auto item)
  {
    m_property_tree->SetItem(item);
    emit InstructionSelected(GetSelectedInstruction());
  };
  connect(m_instruction_tree, &InstructionTreeWidget::InstructionSelected, on_selection_changed);

  connect(m_instruction_tree, &InstructionTreeWidget::InsertAfterRequest, m_composer_actions.get(),
          &ComposerActions::InsertInstructionAfterRequest);
  connect(m_instruction_tree, &InstructionTreeWidget::InsertIntoRequest, m_composer_actions.get(),
          &ComposerActions::InsertInstructionIntoRequest);

  // Remove instruction
  auto on_remove = [this]()
  {
    if (auto item = m_instruction_tree->GetSelectedInstruction(); item)
    {
      m_model->RemoveItem(item);
    }
  };
  connect(m_instruction_tree, &InstructionTreeWidget::RemoveSelectedRequest, this, on_remove);
}

}  // namespace sequencergui
