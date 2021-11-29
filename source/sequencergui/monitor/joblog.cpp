/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/joblog.h"

#include "sequencergui/monitor/messagepanel.h"

namespace sequi
{

JobLog::JobLog() : m_message_panel(nullptr) {}

void JobLog::setMessagePanel(MessagePanel* message_panel)
{
  m_message_panel = message_panel;
  if (!m_message_panel)
  {
    return;
  }

  m_message_panel->onClearLog();

  for (auto& record : m_records)
  {
        m_message_panel->onMessage(QString::fromStdString(record.m_text),
                                  QColor(Qt::gray));
  }
}

void JobLog::append(const std::string& text, EMessageType type)
{
  m_records.push_back({text, type});

  if (m_message_panel)
  {
    m_message_panel->onMessage(QString::fromStdString(text), QColor(Qt::gray));
  }
}

void JobLog::clearLog()
{
  m_records.clear();
  if (m_message_panel)
  {
    m_message_panel->onClearLog();
  }
}

};  // namespace sequi
