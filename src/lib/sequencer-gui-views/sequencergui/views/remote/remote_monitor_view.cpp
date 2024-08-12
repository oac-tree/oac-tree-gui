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

#include <QDebug>
#include <QLineEdit>
#include <QVBoxLayout>

namespace sequencergui
{

RemoteMonitorView::RemoteMonitorView(QWidget *parent) : QWidget(parent), m_line_edit(new QLineEdit)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_line_edit);

  connect(m_line_edit, &QLineEdit::editingFinished, this,
          [this]() { qDebug() << m_line_edit->text(); });
}

}  // namespace sequencergui
