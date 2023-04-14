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

#include "workspace_editor_widget.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/top_items_tree_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMenu>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace sequencergui
{

WorkspaceEditorWidget::WorkspaceEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_insert_after_menu(CreateInsertAfterMenu())
    , m_tree_view(new mvvm::TopItemsTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  connect(m_tree_view, &::mvvm::TopItemsTreeView::SelectedItemChanged, this,
          [this](auto) { emit VariableSelected(GetSelectedVariable()); });

  SetupActions();
}

WorkspaceEditorWidget::~WorkspaceEditorWidget() = default;

void WorkspaceEditorWidget::SetProcedure(ProcedureItem *procedure)
{
  m_tree_view->SetItem(procedure ? procedure->GetWorkspace() : nullptr);
}

VariableItem *WorkspaceEditorWidget::GetSelectedVariable() const
{
  return m_tree_view->GetSelected<VariableItem>();
}

void WorkspaceEditorWidget::SetupActions()
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
  insert_after_button->setToolTip("Insert variable after current selection");
  m_insert_after_action = new QWidgetAction(this);
  m_insert_after_action->setDefaultWidget(insert_after_button);
  addAction(m_insert_after_action);

  auto remove_button = new QToolButton;
  remove_button->setText("Into");
  remove_button->setIcon(styleutils::GetIcon("beaker-remove-outline.svg"));
  remove_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  remove_button->setToolTip("Remove currently selected variable");
  connect(remove_button, &QToolButton::clicked, this, &WorkspaceEditorWidget::RemoveSelectedRequest);
  m_insert_after_action = new QWidgetAction(this);
  m_insert_after_action->setDefaultWidget(remove_button);
  addAction(m_insert_after_action);
}

//! Creates menu to insert Variables in a workspace.
std::unique_ptr<QMenu> WorkspaceEditorWidget::CreateInsertAfterMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainVariableNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit InsertAfterRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace sequencergui
