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

#include "attribute_editor_actions.h"

#include <sequencergui/components/anyvalue_editor_dialog_factory.h>
#include <sequencergui/composer/attribute_editor_action_handler.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/operation/workspace_view_component_provider.h>
#include <sequencergui/pvmonitor/workspace_editor_action_handler.h>
#include <sequencergui/pvmonitor/workspace_editor_actions.h>
#include <sequencergui/pvmonitor/workspace_editor_context.h>
#include <sequencergui/viewmodel/workspace_editor_viewmodel.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/components/visibility_agent_base.h>
#include <sup/gui/core/message_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/style_utils.h>
#include <sup/gui/widgets/tree_helper.h>

#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <QClipboard>
#include <QGuiApplication>
#include <QLineEdit>
#include <QMenu>
#include <QMimeData>
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

WorkspaceEditorWidget::WorkspaceEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_custom_header(
          new sup::gui::CustomHeaderView(kHeaderStateSettingName, kDefaultColumnStretch, this))
    , m_component_provider(std::make_unique<WorkspaceViewComponentProvider>(
          std::make_unique<WorkspaceEditorViewModel>(nullptr), m_tree_view))
    , m_action_handler(new WorkspaceEditorActionHandler(CreateWorkspaceEditorContext(), this))
    , m_editor_actions(new WorkspaceEditorActions(this))
    , m_attribute_action_handler(
          new AttributeEditorActionHandler(CreateAttributeEditorContext(), this))
    , m_attribute_actions(new AttributeEditorActions(m_attribute_action_handler, this))
    , m_line_edit(new QLineEdit)
{
  setWindowTitle("Workspace");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);
  layout->addWidget(m_line_edit);

  m_line_edit->setClearButtonEnabled(true);
  m_line_edit->setPlaceholderText("Filter pattern");

  sup::gui::utils::BeautifyTreeStyle(m_tree_view);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          &WorkspaceEditorWidget::OnTreeContextMenuRequest);

  SetupConnections();

  auto editor_toolbar_actions = m_editor_actions->GetActions(
      {WorkspaceEditorActions::ActionKey::kAdd, WorkspaceEditorActions::ActionKey::kRemove});
  auto attribute_toolbar_actions =
      m_attribute_actions->GetActions({AttributeEditorActions::ActionKey::kModifyAttribute,
                                       AttributeEditorActions::ActionKey::kEditAnyValue});
  addActions(editor_toolbar_actions + attribute_toolbar_actions);

  auto on_subscribe = [this]() { SetWorkspaceItemIntern(m_workspace_item); };

  auto on_unsubscribe = [this]() { SetWorkspaceItemIntern(nullptr); };

  // will be deleted as a child of QObject
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);

  m_editor_actions->RegisterActionsForContext(sup::gui::AppRegisterWidgetUniqueId(this));

  auto on_text = [this]() { m_component_provider->SetFilterPattern(m_line_edit->text()); };
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
  return m_component_provider->GetSelectedItem();
}

void WorkspaceEditorWidget::AdjustTreeAppearance()
{
  m_tree_view->expandAll();
  m_custom_header->AdjustColumnsWidth();
}

void WorkspaceEditorWidget::OnTreeContextMenuRequest(const QPoint &point)
{
  QMenu menu;

  // populate cut/copy/paste menu
  m_editor_actions->SetupMenu(menu, m_action_handler);

  // populate attribute menu
  menu.addSeparator();
  auto attribute_menu = menu.addMenu("Modify attribute");
  m_attribute_actions->SetupMenu(*attribute_menu);

  // populate tree menu
  menu.addSeparator();
  auto collapse_menu = menu.addMenu("Tree settings");
  sup::gui::SetupCollapseExpandMenu(point, *collapse_menu, *m_tree_view);
  menu.exec(m_tree_view->mapToGlobal(point));
}

void WorkspaceEditorWidget::SetWorkspaceItemIntern(WorkspaceItem *workspace_item)
{
  m_component_provider->SetItem(workspace_item);

  if (workspace_item)
  {
    AdjustTreeAppearance();
  }
}

void WorkspaceEditorWidget::SetupConnections()
{
  // propagate variable related requests from WorkspaceEditorActions to WorkspaceEditorActionHandler
  connect(m_editor_actions, &WorkspaceEditorActions::AddVariableRequest, m_action_handler,
          [this](auto str) { m_action_handler->OnAddVariableRequest(str.toStdString()); });
  connect(m_editor_actions, &WorkspaceEditorActions::RemoveVariableRequest, m_action_handler,
          &WorkspaceEditorActionHandler::OnRemoveVariableRequest);
  connect(m_editor_actions, &WorkspaceEditorActions::CutRequest, m_action_handler,
          &WorkspaceEditorActionHandler::Cut);
  connect(m_editor_actions, &WorkspaceEditorActions::CopyRequest, m_action_handler,
          &WorkspaceEditorActionHandler::Copy);
  connect(m_editor_actions, &WorkspaceEditorActions::PasteRequest, m_action_handler,
          &WorkspaceEditorActionHandler::Paste);

  connect(m_attribute_actions, &AttributeEditorActions::EditAnyvalueRequest, m_action_handler,
          &WorkspaceEditorActionHandler::OnEditAnyValueRequest);

  // make inserted item selected, and tree branch expanded
  auto on_select_variable_request = [this](auto item)
  {
    m_component_provider->SetSelectedItem(item);

    auto index_of_inserted = m_component_provider->GetViewModel()->GetIndexOfSessionItem(item);
    if (!index_of_inserted.empty())
    {
      m_tree_view->setExpanded(index_of_inserted.front(), true);
    }
  };
  connect(m_action_handler, &WorkspaceEditorActionHandler::SelectItemRequest, this,
          on_select_variable_request);
}

WorkspaceEditorContext WorkspaceEditorWidget::CreateWorkspaceEditorContext()
{
  WorkspaceEditorContext result;

  auto selected_workspace_callback = [this]() { return m_workspace_item; };
  result.selected_workspace_callback = selected_workspace_callback;

  result.selected_item_callback = [this]() { return GetSelectedItem(); };

  auto send_message_callback = [](const auto &event) { sup::gui::SendWarningMessage(event); };
  result.send_message_callback = send_message_callback;

  result.edit_anyvalue_callback = CreateAnyValueDialogCallback(this);

  result.get_mime_data = []() { return QGuiApplication::clipboard()->mimeData(); };

  result.set_mime_data = [](std::unique_ptr<QMimeData> data)
  { QGuiApplication::clipboard()->setMimeData(data.release()); };

  return result;
}

AttributeEditorContext WorkspaceEditorWidget::CreateAttributeEditorContext()
{
  return {[this]() { return m_component_provider->GetSelectedItem(); }};
}

}  // namespace sequencergui
