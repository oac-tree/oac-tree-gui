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

#ifndef SEQUENCERGUI_OPERATION_MESSAGE_PANEL_H_
#define SEQUENCERGUI_OPERATION_MESSAGE_PANEL_H_

#include <sequencergui/jobsystem/log_event.h>

#include <QWidget>
#include <map>
#include <memory>

class QAction;
class QTreeView;
class QSortFilterProxyModel;
class QToolButton;
class QWidgetAction;

namespace sequencergui
{

class JobLog;
class JobLogViewModel;
class SteadyMenu;

class MessagePanel : public QWidget
{
  Q_OBJECT

public:
  explicit MessagePanel(QWidget* parent = nullptr);
  ~MessagePanel() override;

  void SetLog(JobLog* job_log);

private:
  std::unique_ptr<QWidget> CreateSeveritySelectionWidget();
  std::unique_ptr<SteadyMenu> CreateSeverityChoiceMenu();
  void UpdateSeverityFilter();

  QAction* m_remove_selected_action{nullptr};
  QTreeView* m_tree_view{nullptr};
  JobLogViewModel* m_view_model{nullptr};
  QSortFilterProxyModel* m_proxy_model{nullptr};
  QWidgetAction* m_test_action{nullptr};
  std::map<Severity, bool> m_show_severity_flag;
  std::unique_ptr<SteadyMenu> m_selection_menu;
};
}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_MESSAGE_PANEL_H_
