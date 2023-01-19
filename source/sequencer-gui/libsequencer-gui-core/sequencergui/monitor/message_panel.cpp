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

#include "sequencergui/monitor/message_panel.h"
#include <sequencergui/viewmodel/job_log_viewmodel.h>

#include <sequencergui/utils/style_utils.h>

#include <QAction>
#include <QScrollBar>
#include <QTextEdit>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
QColor GetColor(sequencergui::Severity message_type)
{
  static const std::map<sequencergui::Severity, std::string> message_to_colorname = {
      {sequencergui::Severity::kInfo, "gray"},
      {sequencergui::Severity::kNotice, "darkorchid"},
      {sequencergui::Severity::kWarning, "orange"},
      {sequencergui::Severity::kError, "firebrick"}};
  auto iter = message_to_colorname.find(message_type);
  return iter == message_to_colorname.end() ? QColor(Qt::red) : QColor(iter->second.c_str());
}
}  // namespace

namespace sequencergui
{

MessagePanel::MessagePanel(QWidget* parent)
    : QWidget(parent)
    , m_text_edit(new QTextEdit)
    , m_remove_selected_action(new QAction(this))
    , m_tree_view(new QTreeView)
    , m_view_model(new JobLogViewModel(nullptr))
{
  setWindowTitle("LOG");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_text_edit);
  layout->addWidget(m_tree_view);

  m_text_edit->setReadOnly(true);
  QFont font("unexistent");
  font.setStyleHint(QFont::Monospace);
  m_text_edit->setFont(font);

  m_remove_selected_action->setIcon(styleutils::GetIcon("beaker-remove-outline.svg"));
  addAction(m_remove_selected_action);

  m_tree_view->setModel(m_view_model);
}

void MessagePanel::SetLog(JobLog *job_log)
{
  m_view_model->SetLog(job_log);
}

//void MessagePanel::OnClearLog()
//{
//  m_text_edit->clear();
//}

//void MessagePanel::OnMessage(const LogEvent& log_event)
//{
//  auto color = GetColor(log_event.severity);

//  auto scrollbar = m_text_edit->verticalScrollBar();
//  bool autoscroll = scrollbar->value() == scrollbar->maximum();
//  m_text_edit->setTextColor(color);
//  m_text_edit->append(QString::fromStdString(log_event.message));
//  if (autoscroll)
//  {
//    auto text_cursor = m_text_edit->textCursor();
//    text_cursor.movePosition(QTextCursor::End);
//    m_text_edit->setTextCursor(text_cursor);
//  }
//}

}  // namespace sequencergui
