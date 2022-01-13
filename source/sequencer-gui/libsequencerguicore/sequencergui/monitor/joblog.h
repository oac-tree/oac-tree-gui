/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
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

#ifndef MVVM_MONITOR_JOBLOG_H
#define MVVM_MONITOR_JOBLOG_H

#include "sequencergui/monitor/job_types.h"

#include <string>
#include <vector>

namespace sequencergui
{

class MessagePanel;

//! Holds all messages of running job in chronological order.
//! When MessagePanel is set, updates it with arriving messages.
class JobLog
{
public:
  JobLog();

  void SetMessagePanel(MessagePanel* message_panel);
  void Append(const std::string& text, MessageType type);
  void ClearLog();

private:
  struct Record
  {
    std::string m_text;
    MessageType m_type;
  };

  std::vector<Record> m_records;
  MessagePanel* m_message_panel;
};

}  // namespace sequi

#endif  // MVVM_MONITOR_JOBLOG_H
