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

#include "instruction_editor_widget.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/utils/container_utils.h>
#include <mvvm/widgets/property_tree_view.h>
#include <mvvm/widgets/top_items_tree_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QSplitter>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace sequencergui
{

InstructionEditorWidget::InstructionEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_insert_into_menu(CreateInsertIntoMenu())
    , m_insert_after_menu(CreateInsertAfterMenu())
    , m_tree_view(new mvvm::TopItemsTreeView)
    , m_property_tree(new mvvm::PropertyTreeView)
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->addWidget(m_tree_view);
  m_splitter->addWidget(m_property_tree);
  m_splitter->setSizes(QList<int>() << 300 << 200);

  layout->addWidget(m_splitter);

  auto on_selected_instruction_changed = [this](auto)
  {
    auto selected = GetSelectedInstruction();
    m_property_tree->SetItem(selected);
    emit InstructionSelected(selected);
  };
  connect(m_tree_view, &::mvvm::TopItemsTreeView::SelectedItemChanged, this,
          on_selected_instruction_changed);

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_tree_view->GetTreeView());

  SetupActions();
}

void InstructionEditorWidget::SetProcedure(ProcedureItem *procedure)
{
  m_tree_view->SetItem(procedure ? procedure->GetInstructionContainer() : nullptr);
}

void InstructionEditorWidget::SetSelectedInstructions(
    const std::vector<InstructionItem *> &instructions)
{
  m_tree_view->SetSelectedItems(::mvvm::utils::CastItems<mvvm::SessionItem>(instructions));
}

std::vector<InstructionItem *> InstructionEditorWidget::GetSelectedInstructions() const
{
  return ::mvvm::utils::CastItems<InstructionItem>(m_tree_view->GetSelectedItems());
}

InstructionItem *InstructionEditorWidget::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

void InstructionEditorWidget::SetupActions()
{
  // we are using QToolButon wrapped into QWidgetAction here because
  // 1. we want to pass around QList<QAction*>
  // 2. QAction with menu doesn't provide InstantPopup capabilities

  auto insert_after_button = new QToolButton;
  insert_after_button->setText("After");
  insert_after_button->setIcon(styleutils::GetIcon("plus-circle-outline"));
  insert_after_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  insert_after_button->setPopupMode(QToolButton::InstantPopup);
  insert_after_button->setMenu(m_insert_after_menu.get());
  insert_after_button->setToolTip("Insert instruction after current selection");
  m_insert_after_action = new QWidgetAction(this);
  m_insert_after_action->setDefaultWidget(insert_after_button);
  addAction(m_insert_after_action);

  auto insert_into_button = new QToolButton;
  insert_into_button->setText("Into");
  insert_into_button->setIcon(styleutils::GetIcon("plus-circle-multiple-outline"));
  insert_into_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  insert_into_button->setPopupMode(QToolButton::InstantPopup);
  insert_into_button->setMenu(m_insert_into_menu.get());
  insert_into_button->setToolTip("Insert instruction into currently selected instruction");
  m_insert_into_action = new QWidgetAction(this);
  m_insert_into_action->setDefaultWidget(insert_into_button);
  addAction(m_insert_into_action);

  auto remove_button = new QToolButton;
  remove_button->setText("Into");
  remove_button->setIcon(styleutils::GetIcon("beaker-remove-outline"));
  remove_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  remove_button->setToolTip("Remove currently selected instruction together with its children");
  connect(remove_button, &QToolButton::clicked, this,
          &InstructionEditorWidget::RemoveSelectedRequest);
  m_remove_action = new QWidgetAction(this);
  m_remove_action->setDefaultWidget(remove_button);
  addAction(m_remove_action);
}

std::unique_ptr<QMenu> InstructionEditorWidget::CreateInsertAfterMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainInstructionNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit InsertAfterRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

//! Creates menu to insert an instruction into currently selected instruction.
//! Code mostly coincides with the code above. However, this duplication is temporary and it
//! will diverge in the future (idea to disable some actions if an operation is not possible).

std::unique_ptr<QMenu> InstructionEditorWidget::CreateInsertIntoMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainInstructionNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit InsertIntoRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace sequencergui
