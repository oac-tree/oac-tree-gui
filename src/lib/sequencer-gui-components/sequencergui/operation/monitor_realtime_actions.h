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

class QMenu;

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
  explicit MonitorRealTimeActions(QObject* parent = nullptr);
  ~MonitorRealTimeActions() override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_MONITOR_REALTIME_ACTIONS_H_
