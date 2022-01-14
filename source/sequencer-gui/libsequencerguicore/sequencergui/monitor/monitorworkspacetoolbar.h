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

#ifndef SEQUENCERGUI_MONITOR_MONITORWORKSPACETOOLBAR_H
#define SEQUENCERGUI_MONITOR_MONITORWORKSPACETOOLBAR_H

#include <QToolBar>
#include <memory>

class QToolButton;
class QLabel;
class QMenu;
class QPushButton;

namespace sequencergui
{
//! A toolbar on top of MonitorWorkspaceWidget.

class MonitorWorkspaceToolBar : public QToolBar
{
  Q_OBJECT

public:
  MonitorWorkspaceToolBar(QWidget* parent = nullptr);
  ~MonitorWorkspaceToolBar();

private:
  void AddDotsMenu();
  void InsertStrech();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MONITORWORKSPACETOOLBAR_H
