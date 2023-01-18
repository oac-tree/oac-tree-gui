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

#include "sequencergui/monitor/job_log.h"

#include <sequencergui/monitor/message_panel.h>

namespace
{
QColor GetColor(sequencergui::Severity message_type)
{
  static const std::map<sequencergui::Severity, std::string> message_to_colorname = {
      {sequencergui::Severity::kInfo, "gray"},
      {sequencergui::Severity::kNotice, "darkorchid"},
      {sequencergui::Severity::kWarning, "orange"},
      {sequencergui::Severity::kError, "firebrick"}};
  auto it = message_to_colorname.find(message_type);
  return it == message_to_colorname.end() ? QColor(Qt::red) : QColor(it->second.c_str());
}
}  // namespace

namespace sequencergui
{

JobLog::JobLog() : m_message_panel(nullptr) {}

void JobLog::SetMessagePanel(MessagePanel* message_panel)
{
  m_message_panel = message_panel;
  if (!m_message_panel)
  {
    return;
  }

  m_message_panel->onClearLog();

  // Write all accumulated messages to MessagePanel
  for (auto& record : m_records)
  {
    m_message_panel->onMessage(QString::fromStdString(record.message), QColor(Qt::gray));
  }
}

void JobLog::Append(const LogEvent& log_event)
{
  m_records.push_back(log_event);

  if (m_message_panel)
  {
    m_message_panel->onMessage(QString::fromStdString(log_event.message),
                               GetColor(log_event.severity));
  }
}

void JobLog::ClearLog()
{
  m_records.clear();
  if (m_message_panel)
  {
    m_message_panel->onClearLog();
  }
}

};  // namespace sequencergui
