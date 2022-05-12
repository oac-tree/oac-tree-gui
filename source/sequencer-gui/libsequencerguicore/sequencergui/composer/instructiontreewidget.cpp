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

#include "sequencergui/composer/instructiontreewidget.h"

#include "sequencergui/utils/styleutils.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/procedureitems.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/widgets/topitemstreeview.h"
#include "mvvm/widgets/widgetutils.h"

#include <QToolButton>
#include <QVBoxLayout>

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
  layout->setMargin(0);
  layout->addWidget(m_tree_view);

  connect(m_tree_view, &::mvvm::TopItemsTreeView::SelectedItemChanged, this,
          [this](auto) { emit InstructionSelected(GetSelectedInstruction()); });
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
  return m_tree_view->GetSelectedItems<InstructionItem>();
}

InstructionItem *InstructionTreeWidget::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

QList<QWidget *> InstructionTreeWidget::GetToolBarWidgets()
{
  QList<QWidget *> result;

  auto insert_after_button = new QToolButton;
  insert_after_button->setText("After");
  insert_after_button->setIcon(StyleUtils::GetIcon("plus-circle-outline.svg"));
  insert_after_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  insert_after_button->setPopupMode(QToolButton::InstantPopup);
  insert_after_button->setMenu(m_insert_after_menu.get());
  insert_after_button->setToolTip("Insert instruction after current selection");
  result.push_back(insert_after_button);

  auto insert_into_button = new QToolButton;
  insert_into_button->setText("Into");
  insert_into_button->setIcon(StyleUtils::GetIcon("plus-circle-multiple-outline.svg"));
  insert_into_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  insert_into_button->setPopupMode(QToolButton::InstantPopup);
  insert_into_button->setMenu(m_insert_into_menu.get());
  insert_into_button->setToolTip("Insert instruction into currently selected instruction");
  result.push_back(insert_into_button);

  auto remove_button = new QToolButton;
  remove_button->setText("Into");
  remove_button->setIcon(StyleUtils::GetIcon("beaker-remove-outline.svg"));
  remove_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  remove_button->setToolTip("Remove currently selected instruction together with its children");
  connect(remove_button, &QToolButton::clicked, this,
          &InstructionTreeWidget::RemoveSelectedRequest);
  result.push_back(remove_button);

  return result;
}

std::unique_ptr<QMenu> InstructionTreeWidget::CreateInsertAfterMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::DomainUtils::GetDomainInstructionNames());
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

  auto names = mvvm::utils::GetStringList(sequencergui::DomainUtils::GetDomainInstructionNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit InsertIntoRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace sequencergui
