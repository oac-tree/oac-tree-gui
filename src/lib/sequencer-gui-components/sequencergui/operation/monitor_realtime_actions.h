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

#ifndef SEQUENCERGUI_OPERATION_MONITOR_REALTIME_ACTIONS_H_
#define SEQUENCERGUI_OPERATION_MONITOR_REALTIME_ACTIONS_H_

#include <QObject>
#include <map>
#include <memory>

class QMenu;
class QAction;
class QWidgetAction;
class QToolButton;

namespace sup::gui
{
class ActionMenu;
}

namespace sequencergui
{

/**
 * @brief The MonitorRealTimeActions class holds collection of actions to start/stop/pause job
 * execution.
 */
class MonitorRealTimeActions : public QObject
{
  Q_OBJECT

public:
  enum class ActionKey
  {
    kRun,
    kPause,
    kStep,
    kStop,
    kDelay,
    kSettings,
    kTotalCount
  };

  explicit MonitorRealTimeActions(QObject* parent = nullptr);
  ~MonitorRealTimeActions() override;

  int GetCurrentTickTimeout();

signals:
  void RunRequest();
  void PauseRequest();
  void StepRequest();
  void StopRequest();
  void ChangeDelayRequest(int msec);
  void ScrollToSelectionRequest(bool);

private:
  void ReadSettings();
  void WriteSettings();
  std::unique_ptr<QMenu> CreateDelayMenu();
  std::unique_ptr<QMenu> CreateSettingsMenu();

  QAction* m_run_action{nullptr};
  QAction* m_pause_action{nullptr};
  QAction* m_step_action{nullptr};
  QAction* m_stop_action{nullptr};
  QToolButton* m_delay_button{nullptr};
  QWidgetAction* m_delay_action{nullptr};
  sup::gui::ActionMenu* m_settings_action{nullptr};

  std::unique_ptr<QMenu> m_delay_menu;
  std::unique_ptr<QMenu> m_settings_menu;

  int m_current_tick_timeout{0};
  std::map<ActionKey, QAction*> m_action_map;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_MONITOR_REALTIME_ACTIONS_H_
