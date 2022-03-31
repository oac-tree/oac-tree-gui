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

#ifndef SEQUENCERGUI_MONITOR_MONITORREALTIMETOOLBAR_H
#define SEQUENCERGUI_MONITOR_MONITORREALTIMETOOLBAR_H

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

  static int GetDefaultDelay();

signals:
  void runRequest();
  void pauseRequest();
  void stepRequest();
  void stopRequest();
  void changeDelayRequest(int msec);
  void appChangeRequest(int id);

private:
  void AddDotsMenu();
  void InsertStrech();
  std::unique_ptr<QMenu> CreateDelayMenu();
  std::unique_ptr<QMenu> CreateAppMenu();

  QToolButton* m_run_button{nullptr};
  QToolButton* m_pause_button{nullptr};
  QToolButton* m_step_button{nullptr};
  QToolButton* m_stop_button{nullptr};
  QToolButton* m_delay_button{nullptr};
  std::unique_ptr<QMenu> m_delay_menu;
  std::unique_ptr<QMenu> m_app_menu;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MONITORREALTIMETOOLBAR_H
