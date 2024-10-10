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

#ifndef SEQUENCERGUI_VIEWS_PVMONITOR_MONITOR_WIDGET_ACTIONS_H_
#define SEQUENCERGUI_VIEWS_PVMONITOR_MONITOR_WIDGET_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QObject>

namespace sequencergui
{
class WorkspaceItem;
}

namespace sequencergui
{

/**
 * @brief The MonitorWidgetActions class defines main actions of MonitorWidget.
 */
class MonitorWidgetActions : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief The ActionKey enum defines keys for all available actions.
   */
  enum class ActionKey : std::uint8_t
  {
    kStartWorkspace,
    kStopWorkspace
  };

  explicit MonitorWidgetActions(QObject* parent = nullptr);

  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

  void SetIsRunning(bool is_running);

signals:
  void StartMonitoringRequest();
  void StopMonitoringRequest();

private:
  sup::gui::ActionMap<ActionKey> m_action_map;
  QAction* m_start_action{nullptr};
  QAction* m_stop_action{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_PVMONITOR_MONITOR_WIDGET_ACTIONS_H_
