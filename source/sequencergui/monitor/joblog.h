/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef MVVM_MONITOR_JOBLOG_H
#define MVVM_MONITOR_JOBLOG_H

#include <string>
#include <vector>

namespace sequi
{

class MessagePanel;

class JobLog
{
public:
  enum EMessageType
  {
    kDefault,
    kSuccess,
    kHighlight,
    kWarning,
    kError
  };

  JobLog();

  void setMessagePanel(MessagePanel* message_panel);
  void append(const std::string& text, EMessageType type);
  void clearLog();

private:
  struct Record
  {
    std::string m_text;
    EMessageType m_type;
  };

  std::vector<Record> m_records;
  MessagePanel* m_message_panel;
};

}  // namespace sequi

#endif  // MVVM_MONITOR_JOBLOG_H
