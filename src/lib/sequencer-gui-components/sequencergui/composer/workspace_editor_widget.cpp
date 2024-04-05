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

#include "attribute_editor_action_handler.h"
#include "attribute_editor_actions.h"

#include <sequencergui/components/anyvalue_editor_dialog_factory.h>
#include <sequencergui/components/message_helper.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/workspace_editor_action_handler.h>
#include <sequencergui/pvmonitor/workspace_editor_actions.h>
#include <sequencergui/pvmonitor/workspace_editor_context.h>
#include <sequencergui/viewmodel/workspace_editor_viewmodel.h>
#include <sup/gui/components/visibility_agent_base.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/style_utils.h>
#include <sup/gui/widgets/tree_helper.h>

#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QClipboard>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMenu>
#include <QMimeData>
#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("WorkspaceEditorWidget");
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";

}  // namespace

namespace sequencergui
{

WorkspaceEditorWidget::WorkspaceEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_component_provider(mvvm::CreateProvider<WorkspaceEditorViewModel>(m_tree_view))
    , m_action_handler(new WorkspaceEditorActionHandler(CreateWorkspaceEditorContext(), this))
    , m_editor_actions(new WorkspaceEditorActions(this))
    , m_attribute_action_handler(
          new AttributeEditorActionHandler(CreateAttributeEditorContext(), this))
    , m_attribute_actions(new AttributeEditorActions(m_attribute_action_handler, this))
{
  setWindowTitle("Workspace");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

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

  ReadSettings();

  auto on_subscribe = [this]() { SetProcedureIntern(m_procedure); };

  auto on_unsubscribe = [this]() { SetProcedureIntern(nullptr); };

  // will be deleted as a child of QObject
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

WorkspaceEditorWidget::~WorkspaceEditorWidget()
{
  WriteSettings();
}

void WorkspaceEditorWidget::SetProcedure(ProcedureItem *procedure)
{
  if (procedure == m_procedure)
  {
    return;
  }

  m_procedure = procedure;

  if (m_procedure && isVisible())
  {
    SetProcedureIntern(m_procedure);
  }
}

mvvm::SessionItem *WorkspaceEditorWidget::GetSelectedItem() const
{
  return m_component_provider->GetSelectedItem();
}

void WorkspaceEditorWidget::keyPressEvent(QKeyEvent *event)
{
  if (event == QKeySequence::Cut)
  {
    m_action_handler->Cut();
  }
  else if (event == QKeySequence::Copy)
  {
    m_action_handler->Copy();
  }
  else if (event == QKeySequence::Paste)
  {
    m_action_handler->Paste();
  }
  else
  {
    QWidget::keyPressEvent(event);
  }
}

void WorkspaceEditorWidget::ReadSettings()
{
  QSettings settings;
  if (settings.contains(kHeaderStateSettingName))
  {
    m_custom_header->SetAsFavoriteState(settings.value(kHeaderStateSettingName).toByteArray());
  }
}

void WorkspaceEditorWidget::WriteSettings()
{
  QSettings settings;
  if (m_custom_header->HasFavoriteState())
  {
    settings.setValue(kHeaderStateSettingName, m_custom_header->GetFavoriteState());
  }
}

void WorkspaceEditorWidget::AdjustTreeAppearance()
{
  m_tree_view->expandAll();

  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  else
  {
    sup::gui::AdjustWidthOfColumns(*m_tree_view, {3, 2, 2});
  }
}

void WorkspaceEditorWidget::OnTreeContextMenuRequest(const QPoint &point)
{
  QMenu menu;

  // populate attribute menu
  m_attribute_actions->SetupMenu(menu);

  // populate cut/copy/paste menu
  m_editor_actions->SetupMenu(menu, m_action_handler);

  // populate tree menu
  menu.addSeparator();
  auto collapse_menu = menu.addMenu("Tree settings");
  sup::gui::SetupCollapseExpandMenu(point, *collapse_menu, *m_tree_view);
  menu.exec(m_tree_view->mapToGlobal(point));
}

void WorkspaceEditorWidget::SetProcedureIntern(ProcedureItem *procedure)
{
  if (procedure)
  {
    m_component_provider->SetItem(procedure->GetWorkspace());
    AdjustTreeAppearance();
  }
  else
  {
    m_component_provider->SetItem(nullptr);
  }
}

void WorkspaceEditorWidget::SetupConnections()
{
  // propagate variable related requests from WorkspaceEditorActions to WorkspaceEditorActionHandler
  connect(m_editor_actions, &WorkspaceEditorActions::AddVariableRequest, m_action_handler,
          &WorkspaceEditorActionHandler::OnAddVariableRequest);
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

  auto selected_workspace_callback = [this]()
  { return m_procedure ? m_procedure->GetWorkspace() : nullptr; };
  result.selected_workspace_callback = selected_workspace_callback;

  result.selected_item_callback = [this]() { return GetSelectedItem(); };

  auto send_message_callback = [](const auto &event) { SendWarningMessage(event); };
  result.send_message_callback = send_message_callback;

  result.edit_anyvalue_callback = CreateAnyValueDialogCallback(this);

  result.get_mime_data = []() { return QGuiApplication::clipboard()->mimeData(); };

  result.set_mime_data = [](std::unique_ptr<QMimeData> data)
  { return QGuiApplication::clipboard()->setMimeData(data.release()); };

  return result;
}

AttributeEditorContext WorkspaceEditorWidget::CreateAttributeEditorContext()
{
  return {[this]() { return m_component_provider->GetSelectedItem(); }};
}

}  // namespace sequencergui
