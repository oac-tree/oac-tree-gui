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

#include "operation_workspace_panel.h"

#include <sequencergui/components/visibility_agent_base.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/viewmodel/workspace_operation_viewmodel.h>
#include <sequencergui/widgets/style_utils.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/tree_helper.h>

#include <mvvm/widgets/item_view_component_provider.h>
#include <mvvm/widgets/widget_utils.h>

#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("OperationWorkspacePanel");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";
}  // namespace

namespace sequencergui
{

OperationWorkspacePanel::OperationWorkspacePanel(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_component_provider(mvvm::CreateProvider<WorkspaceOperationViewModel>(m_tree_view))
{
  setWindowTitle("Workspace");

  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tree_view);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_tree_view);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setHeader(m_custom_header);
  m_tree_view->header()->setStretchLastSection(true);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          sup::gui::CreateOnCustomMenuCallback(*m_tree_view));

  ReadSettings();

  auto on_subscribe = [this]() { SetProcedureIntern(m_procedure); };

  auto on_unsubscribe = [this]() { SetProcedureIntern(nullptr); };

  // will be deleted as a child of QObject
  m_visibility_agent = new VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

OperationWorkspacePanel::~OperationWorkspacePanel()
{
  WriteSettings();
}

void OperationWorkspacePanel::SetProcedure(ProcedureItem *procedure)
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

void OperationWorkspacePanel::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kHeaderStateSettingName))
  {
    m_custom_header->SetAsFavoriteState(settings.value(kHeaderStateSettingName).toByteArray());
  }
}

void OperationWorkspacePanel::WriteSettings()
{
  QSettings settings;
  if (m_custom_header->HasFavoriteState())
  {
    settings.setValue(kHeaderStateSettingName, m_custom_header->GetFavoriteState());
  }
}

void OperationWorkspacePanel::AdjustTreeAppearance()
{
  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  m_tree_view->expandAll();
}

void OperationWorkspacePanel::SetProcedureIntern(ProcedureItem *procedure)
{
  m_component_provider->SetItem(procedure ? procedure->GetWorkspace() : nullptr);

  if (procedure)
  {
    AdjustTreeAppearance();
  }
}

}  // namespace sequencergui
