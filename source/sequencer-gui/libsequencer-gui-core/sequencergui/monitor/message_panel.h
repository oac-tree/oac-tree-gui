/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_MESSAGE_PANEL_H_
#define SEQUENCERGUI_MONITOR_MESSAGE_PANEL_H_

#include <sequencergui/jobsystem/log_event.h>

#include <QWidget>

class QAction;
class QTreeView;
class QSortFilterProxyModel;

namespace sequencergui
{

class JobLog;
class JobLogViewModel;

class MessagePanel : public QWidget
{
public:
  explicit MessagePanel(QWidget* parent = nullptr);

  void SetLog(JobLog* job_log);

private:
  QAction* m_remove_selected_action{nullptr};
  QTreeView* m_tree_view{nullptr};
  JobLogViewModel* m_view_model{nullptr};
  QSortFilterProxyModel* m_proxy_model{nullptr};
};
}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MESSAGE_PANEL_H_
