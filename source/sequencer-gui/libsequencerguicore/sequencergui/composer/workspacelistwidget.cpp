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

#include "sequencergui/composer/workspacelistwidget.h"

#include "sequencergui/utils/styleutils.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/procedureitems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/widgets/topitemstreeview.h"
#include "mvvm/widgets/widgetutils.h"

#include <QMenu>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{

WorkspaceListWidget::WorkspaceListWidget(QWidget *parent)
    : QWidget(parent)
    , m_insert_after_menu(CreateInsertAfterMenu())
    , m_tree_view(new mvvm::TopItemsTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);
  layout->addWidget(m_tree_view);

  connect(m_tree_view, &::mvvm::TopItemsTreeView::SelectedItemChanged, this,
          [this](auto) { emit VariableSelected(GetSelectedVariable()); });
}

WorkspaceListWidget::~WorkspaceListWidget() = default;

void WorkspaceListWidget::SetProcedure(ProcedureItem *procedure)
{
  m_tree_view->SetItem(procedure ? procedure->GetWorkspace() : nullptr);
}

VariableItem *WorkspaceListWidget::GetSelectedVariable() const
{
  return m_tree_view->GetSelected<VariableItem>();
}

QList<QWidget *> WorkspaceListWidget::GetToolBarWidgets()
{
  QList<QWidget *> result;

  auto insert_after_button = new QToolButton;
  insert_after_button->setText("After");
  insert_after_button->setIcon(StyleUtils::GetIcon("plus-circle-outline.svg"));
  insert_after_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  insert_after_button->setPopupMode(QToolButton::InstantPopup);
  insert_after_button->setMenu(m_insert_after_menu.get());
  insert_after_button->setToolTip("Insert variable after current selection");
  result.push_back(insert_after_button);

  auto remove_button = new QToolButton;
  remove_button->setText("Into");
  remove_button->setIcon(StyleUtils::GetIcon("beaker-remove-outline.svg"));
  remove_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  remove_button->setToolTip("Remove currently selected variable");
  connect(remove_button, &QToolButton::clicked, this, &WorkspaceListWidget::RemoveSelectedRequest);
  result.push_back(remove_button);

  return result;
}

//! Creates menu to insert Variables in a workspace.
std::unique_ptr<QMenu> WorkspaceListWidget::CreateInsertAfterMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::DomainUtils::GetDomainVariableNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit InsertAfterRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace sequencergui
