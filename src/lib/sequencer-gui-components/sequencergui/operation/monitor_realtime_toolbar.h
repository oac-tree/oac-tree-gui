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

#ifndef SEQUENCERGUI_OPERATION_MONITOR_REALTIME_TOOLBAR_H_
#define SEQUENCERGUI_OPERATION_MONITOR_REALTIME_TOOLBAR_H_

#include <QToolBar>
#include <memory>

class QToolButton;
class QLineEdit;
class QMenu;

namespace sequencergui
{
//! A toolbar on top of MonitorRealTimeWidget with control elements to start/stop execution.

class MonitorRealTimeToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit MonitorRealTimeToolBar(QWidget* parent = nullptr);
  ~MonitorRealTimeToolBar() override;

  int GetCurrentTickTimeout();

signals:
  void runRequest();
  void pauseRequest();
  void stepRequest();
  void stopRequest();
  void changeDelayRequest(int msec);
  void scrollToSelectionRequest(bool);

private:
  void ReadSettings();
  void WriteSettings();
  std::unique_ptr<QMenu> CreateDelayMenu();
  std::unique_ptr<QMenu> CreateSettingsMenu();

  QToolButton* m_run_button{nullptr};
  QToolButton* m_pause_button{nullptr};
  QToolButton* m_step_button{nullptr};
  QToolButton* m_stop_button{nullptr};
  QToolButton* m_delay_button{nullptr};
  QToolButton* m_settings_button{nullptr};
  std::unique_ptr<QMenu> m_delay_menu;
  std::unique_ptr<QMenu> m_settings_menu;

  int m_current_tick_timeout{0};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_MONITOR_REALTIME_TOOLBAR_H_
