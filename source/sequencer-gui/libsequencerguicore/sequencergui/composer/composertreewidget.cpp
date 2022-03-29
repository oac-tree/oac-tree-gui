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

#include "sequencergui/composer/composertreewidget.h"

#include "sequencergui/composer/composertreetoolbar.h"
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
ComposerTreeWidget::ComposerTreeWidget(QWidget* parent)
    : QWidget(parent)
    , m_tool_bar(new ComposerTreeToolBar)
    , m_tab_widget(new QTabWidget)
    , m_instruction_tree(new mvvm::TopItemsTreeView)
    , m_workspace_tree(new mvvm::AllItemsTreeView)
    , m_property_tree(new mvvm::PropertyTreeView)
    , m_splitter(new QSplitter)
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
}

ComposerTreeWidget::~ComposerTreeWidget() = default;

void ComposerTreeWidget::SetModel(SequencerModel* model, ProcedureItem* procedure)
{
  m_model = model;
  m_procedure = procedure;

  if (m_procedure)
  {
    m_instruction_tree->SetApplicationModel(m_model);
    m_workspace_tree->SetApplicationModel(m_model);
    m_instruction_tree->SetRootSessionItem(procedure->GetInstructionContainer());
    m_workspace_tree->SetRootSessionItem(procedure->GetWorkspace());
  }
  else
  {
    m_instruction_tree->SetApplicationModel(nullptr);
    m_workspace_tree->SetApplicationModel(nullptr);
  }
}

void ComposerTreeWidget::SetSelected(InstructionItem* instruction)
{
  m_instruction_tree->SetSelected(instruction);
}

void ComposerTreeWidget::SetSelectedInstructions(const std::vector<InstructionItem*>& instructions)
{
  std::vector<mvvm::SessionItem*> items;
  std::copy(instructions.begin(), instructions.end(), std::back_inserter(items));
  m_instruction_tree->SetSelectedItems(items);
}

std::vector<InstructionItem*> ComposerTreeWidget::GetSelectedInstructions() const
{
  auto selected_items = m_instruction_tree->GetSelectedItems();
  return mvvm::utils::CastedItems<InstructionItem>(selected_items);
}

InstructionItem* ComposerTreeWidget::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

void ComposerTreeWidget::SetupConnections()
{
  auto on_selection_changed = [this](auto item)
  {
    m_property_tree->SetItem(item);
    emit InstructionSelected(GetSelectedInstruction());
  };
  connect(m_instruction_tree, &mvvm::TopItemsTreeView::itemSelected, on_selection_changed);

  // insert after
  auto on_insert_after = [this](auto name)
  {
    if (auto item = m_instruction_tree->GetSelectedItem(); item)
    {
      m_model->InsertNewItem(name.toStdString(), item->GetParent(), item->GetTagIndex().Next());
    }
    else
    {
      m_model->InsertNewItem(name.toStdString(), m_procedure->GetInstructionContainer(), {"", -1});
    }
  };
  connect(m_tool_bar, &ComposerTreeToolBar::insertAfterRequest, this, on_insert_after);

  // Insert into
  auto on_insert_into = [this](auto name)
  {
    if (auto item = m_instruction_tree->GetSelectedItem(); item)
    {
      m_model->InsertNewItem(name.toStdString(), item, {"", -1});
    }
  };
  connect(m_tool_bar, &ComposerTreeToolBar::insertIntoRequest, this, on_insert_into);

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
