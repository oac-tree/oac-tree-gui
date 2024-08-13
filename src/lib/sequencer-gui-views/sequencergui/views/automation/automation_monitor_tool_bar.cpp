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

#include "automation_monitor_tool_bar.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace sequencergui
{

AutomationMonitorToolBar::AutomationMonitorToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_status_label(new QLabel)
    , m_line_edit(new QLineEdit)
    , m_connect_button(new QPushButton("Connect"))
    , m_disconnect_button(new QPushButton("Disconnect"))
{
  addWidget(m_status_label);
  addWidget(m_line_edit);
  addWidget(m_connect_button);
  addWidget(m_disconnect_button);
}

}  // namespace sequencergui
