/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_MONITORWORKSPACETOOLBAR_H
#define SEQUENCERGUI_MAINWINDOW_MONITORWORKSPACETOOLBAR_H

#include <QToolBar>
#include <memory>

class QToolButton;
class QLabel;
class QMenu;
class QPushButton;

namespace sequi
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

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_MONITORWORKSPACETOOLBAR_H
