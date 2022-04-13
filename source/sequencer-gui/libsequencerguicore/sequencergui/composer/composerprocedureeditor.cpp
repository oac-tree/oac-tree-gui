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
    , m_instruction_tree(new mvvm::TopItemsTreeView)
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

  SetupConnections();

  // setting up ComposerActions
  m_composer_actions->SetMessageHandler(CreateStdMessageHandler());
  ComposerContext context;
  context.selected_procedure = [this]() { return m_procedure; };
  context.selected_instruction = [this]()
  { return dynamic_cast<InstructionItem*>(m_instruction_tree->GetSelectedItem()); };
  m_composer_actions->SetContext(context);
}

ComposerProcedureEditor::~ComposerProcedureEditor() = default;

void ComposerProcedureEditor::SetModel(SequencerModel* model)
{
  m_model = model;
  m_composer_actions->SetModel(model);
}

void ComposerProcedureEditor::SetProcedure(ProcedureItem* procedure_item)
{
  // FIXME simplify after the refactoring of TopItemsTreeView
  m_procedure = procedure_item;

  if (m_procedure)
  {
    m_instruction_tree->SetApplicationModel(m_model);
    m_workspace_tree->SetApplicationModel(m_model);
    m_instruction_tree->SetRootSessionItem(m_procedure->GetInstructionContainer());
    m_workspace_tree->SetRootSessionItem(m_procedure->GetWorkspace());
  }
  else
  {
    m_instruction_tree->SetApplicationModel(nullptr);
    m_workspace_tree->SetApplicationModel(nullptr);
  }
}

void ComposerProcedureEditor::SetSelectedInstruction(InstructionItem* instruction)
{
  m_instruction_tree->SetSelected(instruction);
}

void ComposerProcedureEditor::SetSelectedInstructions(const std::vector<InstructionItem*>& instructions)
{
  std::vector<mvvm::SessionItem*> items;
  std::copy(instructions.begin(), instructions.end(), std::back_inserter(items));
  m_instruction_tree->SetSelectedItems(items);
}

std::vector<InstructionItem*> ComposerProcedureEditor::GetSelectedInstructions() const
{
  auto selected_items = m_instruction_tree->GetSelectedItems();
  return mvvm::utils::CastedItems<InstructionItem>(selected_items);
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
  connect(m_instruction_tree, &mvvm::TopItemsTreeView::itemSelected, on_selection_changed);

  connect(m_tool_bar, &ComposerTreeToolBar::insertAfterRequest, m_composer_actions.get(),
          &ComposerActions::InsertInstructionAfterRequest);
  connect(m_tool_bar, &ComposerTreeToolBar::insertIntoRequest, m_composer_actions.get(),
          &ComposerActions::InsertInstructionIntoRequest);

  // Remove instruction
  auto on_remove = [this]()
  {
    if (auto item = m_instruction_tree->GetSelectedItem(); item)
    {
      m_model->RemoveItem(item);
    }
  };
  connect(m_tool_bar, &ComposerTreeToolBar::removeSelectedRequest, this, on_remove);
}

}  // namespace sequencergui
