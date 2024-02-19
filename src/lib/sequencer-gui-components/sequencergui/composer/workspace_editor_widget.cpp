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
    , m_editor_actions(new WorkspaceEditorActions(this))
    , m_action_handler(
          std::make_unique<WorkspaceEditorActionHandler>(CreateWorkspaceEditorContext()))
{
  setWindowTitle("Workspace");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  sup::gui::utils::BeautifyTreeStyle(m_tree_view);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setHeader(m_custom_header);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          sup::gui::CreateOnCustomMenuCallback(*m_tree_view));

  SetupConnections();
  addActions(m_editor_actions->GetActions());

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

void WorkspaceEditorWidget::AdjustColumnWidth()
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

void WorkspaceEditorWidget::SetProcedureIntern(ProcedureItem *procedure)
{
  if (procedure)
  {
    m_component_provider->SetItem(procedure->GetWorkspace());
    AdjustColumnWidth();
  }
  else
  {
    m_component_provider->SetItem(nullptr);
  }
}

void WorkspaceEditorWidget::SetupConnections()
{
  // propagate instruction related requests from WorkspaceEditorWidget to WorkspaceEditorActions
  connect(m_editor_actions, &WorkspaceEditorActions::AddVariableRequest, m_action_handler.get(),
          &WorkspaceEditorActionHandler::OnAddVariableRequest);
  connect(m_editor_actions, &WorkspaceEditorActions::RemoveVariableRequest, m_action_handler.get(),
          &WorkspaceEditorActionHandler::OnRemoveVariableRequest);
  connect(m_editor_actions, &WorkspaceEditorActions::EditAnyvalueRequest, m_action_handler.get(),
          &WorkspaceEditorActionHandler::OnEditAnyvalueRequest);

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
  connect(m_action_handler.get(), &WorkspaceEditorActionHandler::SelectItemRequest, this,
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

  return result;
}

}  // namespace sequencergui
