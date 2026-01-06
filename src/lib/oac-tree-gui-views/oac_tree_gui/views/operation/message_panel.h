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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_MESSAGE_PANEL_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_MESSAGE_PANEL_H_

#include <oac_tree_gui/jobsystem/log_event.h>

#include <QStringList>
#include <QWidget>
#include <map>
#include <memory>

class QAction;
class QTreeView;
class QSortFilterProxyModel;
class QToolButton;
class QWidgetAction;

namespace sup::gui
{
class CustomHeaderView;
class SteadyMenu;
}  // namespace sup::gui

namespace oac_tree_gui
{

class JobLog;
class JobLogViewModel;

/**
 * @brief The MessagePanel class shows JobLog information in a log table.
 *
 * The table is implemented as a tree view  with columns: date, time, severity, source
 * and the message. It has a selector to filter out certain severity levels.
 */
class MessagePanel : public QWidget
{
  Q_OBJECT

public:
  explicit MessagePanel(QWidget* parent_widget = nullptr);
  ~MessagePanel() override;

  MessagePanel(const MessagePanel&) = delete;
  MessagePanel& operator=(const MessagePanel&) = delete;
  MessagePanel(MessagePanel&&) = delete;
  MessagePanel& operator=(MessagePanel&&) = delete;

  void SetLog(JobLog* job_log);

private:
  void ReadSettings();
  void WriteSettings();
  std::unique_ptr<QWidget> CreateSeveritySelectorWidget();
  std::unique_ptr<sup::gui::SteadyMenu> CreateSeveritySelectorMenu();

  /**
   * @brief Setup tree view autoscroll in such a way, that if the scrollbar was at the bottom, it
   * will stay at the bottom after adding new records. If the tree view was scrolled somewhere else,
   * it will stay where it was.
   */
  void SetupAutoscroll();

  /**
   * @brief Update regexp severity filter on board of proxy model.
   */
  void UpdateSeverityFilter();

  QTreeView* m_tree_view{nullptr};
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  JobLogViewModel* m_view_model{nullptr};
  QSortFilterProxyModel* m_proxy_model{nullptr};
  QWidgetAction* m_severity_selector_action{nullptr};
  std::map<Severity, bool> m_show_severity_flag;
  std::unique_ptr<sup::gui::SteadyMenu> m_severity_selector_menu;

  //! controls if the tree was scrolled to the bottom to make auto scroll
  bool m_tree_at_the_bottom{false};
  QStringList m_unchecked_severitites;
};
}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_MESSAGE_PANEL_H_
