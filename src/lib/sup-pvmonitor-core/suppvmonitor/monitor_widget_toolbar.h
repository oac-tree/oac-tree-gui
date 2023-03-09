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

#ifndef LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_MONITOR_WIDGET_TOOLBAR_H_
#define LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_MONITOR_WIDGET_TOOLBAR_H_

#include <QToolBar>

class QToolButton;

namespace suppvmonitor
{

class MonitorWidgetToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit MonitorWidgetToolBar(QWidget* parent = nullptr);
  ~MonitorWidgetToolBar() override;

signals:
  void AddVariableRequest(const QString& variable_type_name);
  void EditAnyvalueRequest();
  void RemoveVariableRequest();
  void StartMonitoringRequest();
  void StopMonitoringRequest();

private:
  std::unique_ptr<QMenu> CreateAddVariableMenu();
  void InsertStrech();

  std::unique_ptr<QMenu> m_add_variable_menu;
  QToolButton* m_add_button{nullptr};
  QToolButton* m_remove_button{nullptr};
  QToolButton* m_edit_anyvalue_button{nullptr};
  QToolButton* m_start_button{nullptr};
  QToolButton* m_stop_button{nullptr};
};

}  // namespace suppvmonitor

#endif  // LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_MONITOR_WIDGET_TOOLBAR_H_
