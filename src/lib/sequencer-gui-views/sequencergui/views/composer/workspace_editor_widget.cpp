/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include "workspace_editor.h"

#include <sup/gui/components/tree_helper.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <QLineEdit>
#include <QMenu>
#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{

const QString kHeaderStateSettingName("WorkspaceEditorWidget/header_state");
const std::vector<int> kDefaultColumnStretch({3, 2, 2});

}  // namespace

namespace sequencergui
{

WorkspaceEditorWidget::WorkspaceEditorWidget(WorkspacePresentationType presentation,
                                             QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_custom_header(
          new sup::gui::CustomHeaderView(kHeaderStateSettingName, kDefaultColumnStretch, this))
    , m_line_edit(new QLineEdit)
    , m_editor(new WorkspaceEditor(presentation, m_tree_view, this))
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

  auto on_text = [this]() { m_editor->SetFilterPattern(m_line_edit->text()); };
  connect(m_line_edit, &QLineEdit::textChanged, this, on_text);
}

WorkspaceEditorWidget::~WorkspaceEditorWidget() = default;

void WorkspaceEditorWidget::SetWorkspaceItem(WorkspaceItem *workspace)
{
  if (workspace == m_workspace_item)
  {
    return;
  }

  m_workspace_item = workspace;

  if (m_workspace_item && isVisible())
  {
    SetWorkspaceItemIntern(m_workspace_item);
  }
}

mvvm::SessionItem *WorkspaceEditorWidget::GetSelectedItem() const
{
  return m_editor->GetSelectedItem();
}

void WorkspaceEditorWidget::SetupTree()
{
  sup::gui::utils::BeautifyTreeStyle(m_tree_view);

  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          &WorkspaceEditorWidget::OnTreeContextMenuRequest);
}

void WorkspaceEditorWidget::AdjustTreeAppearance()
{
  m_tree_view->expandAll();
  m_custom_header->AdjustColumnsWidth();
}

void WorkspaceEditorWidget::OnTreeContextMenuRequest(const QPoint &point)
{
  QMenu menu;

  m_editor->SetupContextMenu(menu);

  // populate tree menu
  menu.addSeparator();
  auto collapse_menu = menu.addMenu("Tree settings");
  sup::gui::SetupCollapseExpandMenu(point, *collapse_menu, *m_tree_view);
  menu.exec(m_tree_view->mapToGlobal(point));
}

void WorkspaceEditorWidget::SetWorkspaceItemIntern(WorkspaceItem *workspace_item)
{
  m_editor->SetWorkspaceItem(workspace_item);

  if (workspace_item)
  {
    AdjustTreeAppearance();
  }
}

}  // namespace sequencergui
