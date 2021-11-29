/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
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
