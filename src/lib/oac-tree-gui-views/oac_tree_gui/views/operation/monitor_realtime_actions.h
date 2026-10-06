/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_MONITOR_REALTIME_ACTIONS_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_MONITOR_REALTIME_ACTIONS_H_

#include <sup/gui/components/action_map.h>
#include <sup/gui/settings/scoped_callback.h>

#include <QObject>
#include <cstdint>
#include <memory>

class QMenu;

namespace sup::gui
{
class ISettingsManager;
class ActionMenu;
}  // namespace sup::gui

namespace oac_tree_gui
{

/**
 * @brief The RealTimeInfo struct holds the persistent state of real-time actions.
 */
struct RealTimeInfo
{
  std::int32_t tick_timeout{0};  //!< delay after each instruction status change, msec
};

template <typename Archive>
void Visit(Archive& archive, RealTimeInfo& info)
{
  archive("tick_timeout", info.tick_timeout);
}

/**
 * @brief The MonitorRealTimeActions class holds collection of actions to start/stop/pause job
 * execution.
 */
class MonitorRealTimeActions : public QObject
{
  Q_OBJECT

public:
  enum class ActionKey : std::uint8_t
  {
    kRun,
    kPause,
    kStep,
    kStop,
    kReset,
    kDelay,
    kSettings,
    kTotalCount
  };

  /**
   * @brief Main c-tor.
   *
   * The settings manager must outlive the actions.
   */
  explicit MonitorRealTimeActions(sup::gui::ISettingsManager& settings_manager,
                                  QObject* parent_object = nullptr);
  ~MonitorRealTimeActions() override;

  MonitorRealTimeActions(const MonitorRealTimeActions&) = delete;
  MonitorRealTimeActions& operator=(const MonitorRealTimeActions&) = delete;
  MonitorRealTimeActions(MonitorRealTimeActions&&) = delete;
  MonitorRealTimeActions& operator=(MonitorRealTimeActions&&) = delete;

  int GetCurrentTickTimeout() const;

  void SetCurrentTickTimeout(int msec);

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

signals:
  void RunRequest();
  void PauseRequest();
  void StepRequest();
  void StopRequest();
  void ResetRequest();
  void ChangeDelayRequest(int msec);
  void ScrollToSelectionRequest(bool);

private:
  std::unique_ptr<QMenu> CreateDelayMenu();
  std::unique_ptr<QMenu> CreateSettingsMenu();

  QAction* m_run_action{nullptr};
  QAction* m_pause_action{nullptr};
  QAction* m_step_action{nullptr};
  QAction* m_stop_action{nullptr};
  QAction* m_reset_action{nullptr};
  sup::gui::ActionMenu* m_delay_action{nullptr};
  sup::gui::ActionMenu* m_settings_action{nullptr};

  std::unique_ptr<QMenu> m_delay_menu;
  std::unique_ptr<QMenu> m_settings_menu;

  int m_current_tick_timeout{0};
  sup::gui::ActionMap<ActionKey> m_action_map;
  sup::gui::ScopedCallback m_settings_connection;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_MONITOR_REALTIME_ACTIONS_H_
