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

#ifndef SEQUENCERGUI_VIEWS_REMOTE_AUTOMATION_MONITOR_TOOL_BAR_H_
#define SEQUENCERGUI_VIEWS_REMOTE_AUTOMATION_MONITOR_TOOL_BAR_H_

#include <QToolBar>

class QLineEdit;
class QPushButton;
class QLabel;

namespace sequencergui
{

/**
 * @brief The AutomationMonitorToolBar class is a tool bar with server name and connect/disconnect
 * buttons.
 *
 * Located on top of RemoteMonitorView.
 */
class AutomationMonitorToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit AutomationMonitorToolBar(QWidget* parent = nullptr);

signals:
  void ConnectRequest(const QString& server_name);

private:
  QLabel* m_status_label{nullptr};
  QLineEdit* m_line_edit{nullptr};
  QPushButton* m_connect_button{nullptr};
  QPushButton* m_disconnect_button{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_REMOTE_AUTOMATION_MONITOR_TOOL_BAR_H_
