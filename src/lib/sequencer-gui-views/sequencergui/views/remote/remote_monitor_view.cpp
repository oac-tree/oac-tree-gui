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

#include "remote_monitor_view.h"

#include <sequencergui/automation/automation_manager.h>

#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace sequencergui
{

RemoteMonitorView::RemoteMonitorView(QWidget *parent)
    : QWidget(parent), m_line_edit(new QLineEdit), m_push_button(new QPushButton("Connect"))
{
  auto layout = new QHBoxLayout(this);
  layout->addWidget(m_line_edit);
  layout->addWidget(m_push_button);

  connect(m_line_edit, &QLineEdit::editingFinished, this,
          [this]() { qDebug() << m_line_edit->text(); });

  connect(m_push_button, &QPushButton::clicked, this, [this](auto) { OnConnect(); });
}

RemoteMonitorView::~RemoteMonitorView() = default;

void RemoteMonitorView::OnConnect()
{
  qDebug() << "RemoteMonitorView::OnConnect()";

  m_automation_manager = std::make_unique<AutomationManager>(m_line_edit->text().toStdString());
}

}  // namespace sequencergui
