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

#include "operation_workspace_widget.h"

#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/operation/workspace_view_component_provider.h>
#include <sequencergui/viewmodel/workspace_operation_viewmodel.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/style_utils.h>
#include <sup/gui/widgets/tree_helper.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/widgets/widget_utils.h>

#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("OperationWorkspaceWidget");

QString GetHeaderStateSettingName(sequencergui::OperationWorkspaceWidget::Mode mode)
{
  QString mode_str = QString::number(static_cast<int>(mode));
  return QString("%1_%2/%3").arg(kGroupName, mode_str, "header_state");
}

}  // namespace

namespace sequencergui
{

OperationWorkspaceWidget::OperationWorkspaceWidget(Mode mode, QWidget *parent)
    : QWidget(parent)
    , m_mode(mode)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(GetHeaderStateSettingName(mode), this))
{
  if (mode == Mode::kWorkspaceTree)
  {
    setWindowTitle("Variable Tree");
    m_component_provider = std::make_unique<WorkspaceViewComponentProvider>(
        std::make_unique<mvvm::AllItemsViewModel>(nullptr), m_tree_view);
  }
  else
  {
    setWindowTitle("Variable Table");
    m_component_provider = std::make_unique<WorkspaceViewComponentProvider>(
        std::make_unique<WorkspaceOperationViewModel>(nullptr), m_tree_view);
  }

  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tree_view);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  sup::gui::utils::BeautifyTreeStyle(m_tree_view);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setHeader(m_custom_header);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          sup::gui::CreateOnCustomMenuCallback(*m_tree_view));

  auto on_subscribe = [this]() { SetProcedureIntern(m_procedure); };

  auto on_unsubscribe = [this]() { SetProcedureIntern(nullptr); };

  // will be deleted as a child of QObject
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

OperationWorkspaceWidget::~OperationWorkspaceWidget() = default;

void OperationWorkspaceWidget::SetProcedure(ProcedureItem *procedure)
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

void OperationWorkspaceWidget::SetFilterPattern(const QString &pattern)
{
  m_component_provider->SetFilterPattern(pattern);
}

void OperationWorkspaceWidget::AdjustTreeAppearance()
{
  m_tree_view->expandAll();
  m_custom_header->AdjustColumnsWidth();
}

void OperationWorkspaceWidget::SetProcedureIntern(ProcedureItem *procedure)
{
  m_component_provider->SetItem(procedure ? procedure->GetWorkspace() : nullptr);

  if (procedure)
  {
    AdjustTreeAppearance();
  }
}

}  // namespace sequencergui
