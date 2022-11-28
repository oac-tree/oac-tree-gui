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

#ifndef SUPPVMONITOR_MONITOR_WIDGET_TOOLBAR_H_
#define SUPPVMONITOR_MONITOR_WIDGET_TOOLBAR_H_

#include <QToolBar>

class QToolButton;

namespace suppvmonitor
{

class MonitorWidgetToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit MonitorWidgetToolBar(QWidget* parent = nullptr);

signals:
  void SetupWorkspaceRequest();

private:
  QToolButton* m_setup_workspace_button{nullptr};
};

}  // namespace suppvmonitor

#endif  // SUPPVMONITOR_MONITOR_WIDGET_TOOLBAR_H_
