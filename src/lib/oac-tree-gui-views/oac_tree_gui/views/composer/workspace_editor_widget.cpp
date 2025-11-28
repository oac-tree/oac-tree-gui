/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "workspace_editor_widget.h"

#include "workspace_editor.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/operation/objects/workspace_view_component_provider.h>
#include <oac_tree_gui/viewmodel/workspace_editor_viewmodel.h>
#include <oac_tree_gui/viewmodel/workspace_operation_viewmodel.h>

#include <sup/gui/components/tree_helper.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <QLineEdit>
#include <QMenu>
#include <QSettings>
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

WorkspaceEditorWidget::WorkspaceEditorWidget(sup::gui::IAppCommandService& command_service,
                                             WorkspacePresentationType presentation,
                                             QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(kHeaderStateSettingName,
                                                     GetDefaultColumnStretch(presentation), this))
    , m_component_provider(CreateProvider(presentation))
    , m_line_edit(new QLineEdit)
    , m_editor(new WorkspaceEditor(
          command_service, [this]() { return m_component_provider->GetSelectedItems(); }, this))
    , m_edit_type(WorkspaceEditType::kEditorEnabled)
{
  setWindowTitle("Workspace");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);
  layout->addWidget(m_line_edit);

  m_line_edit->setClearButtonEnabled(true);
  m_line_edit->setPlaceholderText("Filter pattern");

  SetupTree();

  addActions(m_editor->GetToolBarActions());

  auto on_subscribe = [this]() { SetWorkspaceItemIntern(m_workspace_item); };

  auto on_unsubscribe = [this]() { SetWorkspaceItemIntern(nullptr); };

  // will be deleted as a child of QObject
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);

  auto on_text = [this]() { m_component_provider->SetFilterPattern(m_line_edit->text()); };
  connect(m_line_edit, &QLineEdit::textChanged, this, on_text);

  SetupConnections();
}

WorkspaceEditorWidget::~WorkspaceEditorWidget() = default;

void WorkspaceEditorWidget::SetWorkspaceItem(WorkspaceItem* workspace)
{
  if (workspace == m_workspace_item)
  {
    return;
  }

  m_workspace_item = workspace;

  if ((m_workspace_item != nullptr) && isVisible())
  {
    SetWorkspaceItemIntern(m_workspace_item);
  }
}

void WorkspaceEditorWidget::SetWorkspaceEditType(WorkspaceEditType edit_type)
{
  m_edit_type = edit_type;

  m_tree_view->setEditTriggers(edit_type != WorkspaceEditType::kReadOnly
                                   ? QAbstractItemView::DoubleClicked
                                         | QAbstractItemView::SelectedClicked
                                   : QAbstractItemView::NoEditTriggers);

  for (auto action : actions())
  {
    action->setEnabled(edit_type == WorkspaceEditType::kEditorEnabled);
  }
}

void WorkspaceEditorWidget::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
  AdjustTreeAppearance();
}

void WorkspaceEditorWidget::SetupConnections()
{
  auto on_item_select_request = [this](auto item)
  {
    m_component_provider->SetSelectedItem(item);

    auto index_of_inserted = m_component_provider->GetViewIndexes(item);
    if (!index_of_inserted.empty())
    {
      m_tree_view->setExpanded(index_of_inserted.front(), true);
    }
  };
  connect(m_editor, &WorkspaceEditor::ItemSelectRequest, this, on_item_select_request);
}

void WorkspaceEditorWidget::SetupTree()
{
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          &WorkspaceEditorWidget::OnTreeContextMenuRequest);
  m_tree_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
  m_tree_view->expandAll();
}

void WorkspaceEditorWidget::AdjustTreeAppearance()
{
  m_custom_header->AdjustColumnsWidth();
}

std::unique_ptr<WorkspaceViewComponentProvider> WorkspaceEditorWidget::CreateProvider(
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

void WorkspaceEditorWidget::OnTreeContextMenuRequest(const QPoint& point)
{
  QMenu menu;

  if (m_edit_type == WorkspaceEditType::kEditorEnabled)
  {
    m_editor->SetupContextMenu(menu);
  }

  // populate tree menu
  menu.addSeparator();
  auto collapse_menu = menu.addMenu("Tree settings");
  sup::gui::SetupCollapseExpandMenu(point, *collapse_menu, *m_tree_view);
  menu.exec(m_tree_view->mapToGlobal(point));
}

void WorkspaceEditorWidget::SetWorkspaceItemIntern(WorkspaceItem* workspace_item)
{
  m_editor->SetWorkspaceItem(workspace_item);
  m_component_provider->SetItem(workspace_item);

  if (workspace_item != nullptr)
  {
    AdjustTreeAppearance();
  }
}

}  // namespace oac_tree_gui
