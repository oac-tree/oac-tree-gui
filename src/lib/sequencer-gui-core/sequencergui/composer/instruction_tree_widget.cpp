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

#include "sequencergui/composer/instruction_tree_widget.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/utils/style_utils.h>

#include <mvvm/utils/container_utils.h>
#include <mvvm/widgets/top_items_tree_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QToolButton>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace sequencergui
{

InstructionTreeWidget::InstructionTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_insert_into_menu(CreateInsertIntoMenu())
    , m_insert_after_menu(CreateInsertAfterMenu())
    , m_tree_view(new mvvm::TopItemsTreeView)

{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  connect(m_tree_view, &::mvvm::TopItemsTreeView::SelectedItemChanged, this,
          [this](auto) { emit InstructionSelected(GetSelectedInstruction()); });

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_tree_view->GetTreeView());

  SetupActions();
}

void InstructionTreeWidget::SetProcedure(ProcedureItem *procedure)
{
  m_tree_view->SetItem(procedure ? procedure->GetInstructionContainer() : nullptr);
}

void InstructionTreeWidget::SetSelectedInstruction(InstructionItem *instruction)
{
  m_tree_view->SetSelectedItem(instruction);
}

void InstructionTreeWidget::SetSelectedInstructions(
    const std::vector<InstructionItem *> &instructions)
{
  m_tree_view->SetSelectedItems(::mvvm::utils::CastItems<mvvm::SessionItem>(instructions));
}

std::vector<InstructionItem *> InstructionTreeWidget::GetSelectedInstructions() const
{
  return ::mvvm::utils::CastItems<InstructionItem>(m_tree_view->GetSelectedItems());
}

InstructionItem *InstructionTreeWidget::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

void InstructionTreeWidget::SetupActions()
{
  // we are using QToolButon wrapped into QWidgetAction here because
  // 1. we want to pass around QList<QAction*>
  // 2. QAction with menu doesn't provide InstantPopup capabilities

  auto insert_after_button = new QToolButton;
  insert_after_button->setText("After");
  insert_after_button->setIcon(styleutils::GetIcon("plus-circle-outline.svg"));
  insert_after_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  insert_after_button->setPopupMode(QToolButton::InstantPopup);
  insert_after_button->setMenu(m_insert_after_menu.get());
  insert_after_button->setToolTip("Insert instruction after current selection");
  m_insert_after_action = new QWidgetAction(this);
  m_insert_after_action->setDefaultWidget(insert_after_button);
  addAction(m_insert_after_action);

  auto insert_into_button = new QToolButton;
  insert_into_button->setText("Into");
  insert_into_button->setIcon(styleutils::GetIcon("plus-circle-multiple-outline.svg"));
  insert_into_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  insert_into_button->setPopupMode(QToolButton::InstantPopup);
  insert_into_button->setMenu(m_insert_into_menu.get());
  insert_into_button->setToolTip("Insert instruction into currently selected instruction");
  m_insert_into_action = new QWidgetAction(this);
  m_insert_into_action->setDefaultWidget(insert_into_button);
  addAction(m_insert_into_action);

  auto remove_button = new QToolButton;
  remove_button->setText("Into");
  remove_button->setIcon(styleutils::GetIcon("beaker-remove-outline.svg"));
  remove_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  remove_button->setToolTip("Remove currently selected instruction together with its children");
  connect(remove_button, &QToolButton::clicked, this,
          &InstructionTreeWidget::RemoveSelectedRequest);
  m_remove_action = new QWidgetAction(this);
  m_remove_action->setDefaultWidget(remove_button);
  addAction(m_remove_action);
}

std::unique_ptr<QMenu> InstructionTreeWidget::CreateInsertAfterMenu()
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

std::unique_ptr<QMenu> InstructionTreeWidget::CreateInsertIntoMenu()
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
