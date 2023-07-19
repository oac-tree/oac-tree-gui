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

#include "monitor_widget_toolbar.h"

#include "workspace_editor_actions.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/widgets/style_utils.h>

#include <QToolButton>

namespace sequencergui
{

MonitorWidgetToolBar::MonitorWidgetToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_workspace_editor_actions(new WorkspaceEditorActions(this))
    , m_start_button(new QToolButton)
    , m_stop_button(new QToolButton)
{
  setIconSize(sequencergui::styleutils::ToolBarIconSize());

  using styleutils::GetIcon;

  connect(m_workspace_editor_actions, &WorkspaceEditorActions::InsertAfterRequest, this,
          &MonitorWidgetToolBar::AddVariableRequest);
  connect(m_workspace_editor_actions, &WorkspaceEditorActions::EditAnyvalueRequest, this,
          &MonitorWidgetToolBar::EditAnyvalueRequest);
  connect(m_workspace_editor_actions, &WorkspaceEditorActions::RemoveSelectedRequest, this,
          &MonitorWidgetToolBar::RemoveVariableRequest);

  addActions(m_workspace_editor_actions->GetActions());

  addSeparator();

  m_start_button->setText("Start");
  m_start_button->setIcon(GetIcon("chevron-right-circle-outline"));
  m_start_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_start_button->setToolTip("Start monitoring workspace variables");
  connect(m_start_button, &QToolButton::clicked, this,
          &MonitorWidgetToolBar::StartMonitoringRequest);
  addWidget(m_start_button);

  m_stop_button->setText("Stop");
  m_stop_button->setIcon(GetIcon("stop-circle-outline"));
  m_stop_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_stop_button->setToolTip("Stop monitoring");
  connect(m_stop_button, &QToolButton::clicked, this, &MonitorWidgetToolBar::StopMonitoringRequest);
  addWidget(m_stop_button);
}

MonitorWidgetToolBar::~MonitorWidgetToolBar() = default;

void MonitorWidgetToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace sequencergui
