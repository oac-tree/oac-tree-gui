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
    : QToolBar(parent), m_start_button(new QToolButton), m_stop_button(new QToolButton)
{
  setIconSize(sequencergui::styleutils::ToolBarIconSize());

  auto font = m_start_button->font();
  auto psize = font.pointSize() * 1.05;
  font.setPointSize(psize);

  m_start_button->setText("Start");
  m_start_button->setFont(font);
  m_start_button->setIcon(sequencergui::styleutils::GetIcon("chevron-right-circle-outline.svg"));
  m_start_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_start_button->setToolTip("Start monitoring workspace variables");
  connect(m_start_button, &QToolButton::clicked, this,
          &MonitorWidgetToolBar::SetupWorkspaceRequest);
  addWidget(m_start_button);

  m_stop_button->setText("Stop");
  m_stop_button->setFont(font);
  m_stop_button->setIcon(sequencergui::styleutils::GetIcon("stop-circle-outline.svg"));
  m_stop_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_stop_button->setToolTip("Stop monitoring");
  addWidget(m_stop_button);
}

}  // namespace suppvmonitor
