/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/messagepanel.h"

#include "sequencergui/utils/styleutils.h"

#include <QAction>
#include <QScrollBar>
#include <QTextEdit>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{

MessagePanel::MessagePanel(QWidget *parent)
    : QWidget(parent), m_text_edit(new QTextEdit), m_remove_selected_action(new QAction(this))
{
  setWindowTitle("LOG");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);
  layout->addWidget(m_text_edit);

  m_text_edit->setReadOnly(true);
  QFont f("unexistent");
  f.setStyleHint(QFont::Monospace);
  m_text_edit->setFont(f);

  m_remove_selected_action->setIcon(styleutils::GetIcon("beaker-remove-outline.svg"));
  addAction(m_remove_selected_action);
}

void MessagePanel::onClearLog()
{
  m_text_edit->clear();
}

void MessagePanel::onMessage(const QString &message, const QColor &color)
{
  auto scrollbar = m_text_edit->verticalScrollBar();
  bool autoscroll = scrollbar->value() == scrollbar->maximum();
  m_text_edit->setTextColor(color);
  m_text_edit->append(message);
  if (autoscroll)
  {
    QTextCursor c = m_text_edit->textCursor();
    c.movePosition(QTextCursor::End);
    m_text_edit->setTextCursor(c);
  }
}


}  // namespace sequencergui
