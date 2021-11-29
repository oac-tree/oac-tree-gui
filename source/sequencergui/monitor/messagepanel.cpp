/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/messagepanel.h"
#include "sequencergui/mainwindow/explorernarrowtoolbar.h"

#include <QScrollBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QToolButton>

namespace sequi
{

MessagePanel::MessagePanel(QWidget *parent) : CollapsibleWidget(parent), m_text_edit(new QTextEdit)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);
  layout->addWidget(m_text_edit);

  m_text_edit->setReadOnly(true);
  QFont f("unexistent");
  f.setStyleHint(QFont::Monospace);
  m_text_edit->setFont(f);

  SetText("LOGBOOK");

  auto tool_bar = GetToolBar();

  auto remove_selected_button = new QToolButton;
  remove_selected_button->setIcon(QIcon(":/icons/beaker-remove-outline.svg"));
  tool_bar->AddWidget(remove_selected_button);

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

}  // namespace sequi
