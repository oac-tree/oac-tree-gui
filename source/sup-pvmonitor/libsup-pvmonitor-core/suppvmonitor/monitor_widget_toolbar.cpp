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

#include "suppvmonitor/monitor_widget_toolbar.h"

#include <sequencergui/utils/style_utils.h>

#include <QToolButton>

namespace suppvmonitor
{

MonitorWidgetToolBar::MonitorWidgetToolBar(QWidget *parent)
    : QToolBar(parent), m_setup_workspace_button(new QToolButton)
{
  setIconSize(sequencergui::styleutils::ToolBarIconSize());

  auto font = m_setup_workspace_button->font();
  auto psize = font.pointSize() * 1.05;
  font.setPointSize(psize);

  m_setup_workspace_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_setup_workspace_button->setText("Connect");
  m_setup_workspace_button->setFont(font);
  m_setup_workspace_button->setIcon(
      sequencergui::styleutils::GetIcon("chevron-right-circle-outline.svg"));
  connect(m_setup_workspace_button, &QToolButton::clicked, this,
          &MonitorWidgetToolBar::SetupWorkspaceRequest);
  addWidget(m_setup_workspace_button);
}

}  // namespace suppvmonitor
