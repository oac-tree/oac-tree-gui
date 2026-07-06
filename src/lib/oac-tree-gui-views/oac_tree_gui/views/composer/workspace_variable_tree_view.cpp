/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "workspace_variable_tree_view.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/operation/objects/workspace_view_component_provider.h>
#include <oac_tree_gui/viewmodel/workspace_editor_viewmodel.h>
#include <oac_tree_gui/viewmodel/workspace_operation_viewmodel.h>

#include <sup/gui/components/tree_helper.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/model/item_selection.h>

#include <QLineEdit>
#include <QMenu>
#include <QTreeView>
#include <QVBoxLayout>

namespace oac_tree_gui
{

namespace
{

const QString kHeaderStateSettingName("WorkspaceEditorWidget/header_state");

std::vector<std::int32_t> GetDefaultColumnStretch(WorkspacePresentationType presentation)
{
  if (presentation == WorkspacePresentationType::kWorkspaceTree)
  {
    const std::vector<std::int32_t> kDefaultColumnStretch({3, 2, 2});  // name, value, typeName
    return kDefaultColumnStretch;
  }

  if (presentation == WorkspacePresentationType::kWorkspaceTable)
  {
    const std::vector<std::int32_t> kDefaultColumnStretch(
        {3, 2, 2, 2});  // name, value, typeName, Channel
    return kDefaultColumnStretch;
  }

  throw RuntimeException("Unknown presentation");
}

}  // namespace

WorkspaceVariableTreeView::WorkspaceVariableTreeView(WorkspacePresentationType presentation,
                                                     QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(kHeaderStateSettingName,
                                                     GetDefaultColumnStretch(presentation), this))
    , m_component_provider(CreateProvider(presentation))
    , m_line_edit(new QLineEdit)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);
  layout->addWidget(m_line_edit);

  m_line_edit->setClearButtonEnabled(true);
  m_line_edit->setPlaceholderText("Filter pattern");

  SetupTree();

  auto on_subscribe = [this]() { SetWorkspaceItemIntern(m_workspace_item); };
  auto on_unsubscribe = [this]() { SetWorkspaceItemIntern(nullptr); };
  m_visibility_agent =
      std::make_unique<sup::gui::VisibilityAgentBase>(on_subscribe, on_unsubscribe);
  m_visibility_agent->SetTarget(this);

  auto on_text = [this]() { m_component_provider->SetFilterPattern(m_line_edit->text()); };
  connect(m_line_edit, &QLineEdit::textChanged, this, on_text);
}

WorkspaceVariableTreeView::~WorkspaceVariableTreeView()
{
  // we should reset visibility agent since on destruction widget becomes invisible (in
  // uncontrollable moment of time) and agent might try to call invalid callbacks.
  m_visibility_agent.reset();
}

void WorkspaceVariableTreeView::SetWorkspaceItem(WorkspaceItem* workspace_item)
{
  if (workspace_item == m_workspace_item)
  {
    return;
  }

  m_workspace_item = workspace_item;

  if ((m_workspace_item != nullptr) && isVisible())
  {
    SetWorkspaceItemIntern(m_workspace_item);
  }
}

void WorkspaceVariableTreeView::SetEditable(bool value)
{
  m_tree_view->setEditTriggers(value ? QAbstractItemView::DoubleClicked
                                           | QAbstractItemView::SelectedClicked
                                     : QAbstractItemView::NoEditTriggers);
}

std::vector<const mvvm::SessionItem*> WorkspaceVariableTreeView::GetSelectedItems() const
{
  return m_component_provider->GetSelection().GetItems();
}

void WorkspaceVariableTreeView::SelectItem(const mvvm::SessionItem* item)
{
  m_component_provider->SetSelection(mvvm::ItemSelection(item));

  auto index_of_inserted = m_component_provider->GetViewIndexes(item);
  if (!index_of_inserted.empty())
  {
    m_tree_view->setExpanded(index_of_inserted.front(), true);
  }
}

void WorkspaceVariableTreeView::SetContextMenuCallback(std::function<void(QMenu&)> callback)
{
  m_context_menu_callback = std::move(callback);
}

void WorkspaceVariableTreeView::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
  AdjustTreeAppearance();
}

void WorkspaceVariableTreeView::SetupTree()
{
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          &WorkspaceVariableTreeView::OnTreeContextMenuRequest);
  m_tree_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
  m_tree_view->expandAll();
}

void WorkspaceVariableTreeView::AdjustTreeAppearance()
{
  m_custom_header->AdjustColumnsWidth();
}

std::unique_ptr<WorkspaceViewComponentProvider> WorkspaceVariableTreeView::CreateProvider(
    WorkspacePresentationType presentation) const
{
  std::unique_ptr<WorkspaceViewComponentProvider> result;

  if (presentation == WorkspacePresentationType::kWorkspaceTree)
  {
    result = std::make_unique<WorkspaceViewComponentProvider>(
        std::make_unique<WorkspaceEditorViewModel>(nullptr), m_tree_view);
  }
  else if (presentation == WorkspacePresentationType::kWorkspaceTable)
  {
    result = std::make_unique<WorkspaceViewComponentProvider>(
        std::make_unique<WorkspaceOperationViewModel>(nullptr), m_tree_view);
  }
  else
  {
    throw RuntimeException("Unknown presentation");
  }
  return result;
}

void WorkspaceVariableTreeView::OnTreeContextMenuRequest(const QPoint& point)
{
  QMenu menu;

  if (m_context_menu_callback)
  {
    m_context_menu_callback(menu);
  }

  // populate tree menu
  menu.addSeparator();
  auto collapse_menu = menu.addMenu("Tree settings");
  sup::gui::SetupCollapseExpandMenu(point, *collapse_menu, *m_tree_view);
  menu.exec(m_tree_view->mapToGlobal(point));
}

void WorkspaceVariableTreeView::SetWorkspaceItemIntern(WorkspaceItem* workspace_item)
{
  m_component_provider->SetItem(workspace_item);

  if (workspace_item != nullptr)
  {
    AdjustTreeAppearance();
  }
}

}  // namespace oac_tree_gui
