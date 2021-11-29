/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_MONITORTREETOOLBAR_H
#define SEQUENCERGUI_MAINWINDOW_MONITORTREETOOLBAR_H

#include <QToolBar>
#include <memory>

class QToolButton;
class QLineEdit;
class QMenu;

namespace sequi
{
//! A toolbar on top of MonitorTreeWidget with control elements to start/stop execution.

class MonitorTreeToolBar : public QToolBar
{
  Q_OBJECT

public:
  MonitorTreeToolBar(QWidget* parent = nullptr);
  ~MonitorTreeToolBar();

  static int GetDefaultDelay();

signals:
  void runRequest();
  void pauseRequest();
  void stepRequest();
  void stopRequest();
  void changeDelayRequest(int msec);

private:
  void AddDotsMenu();
  void InsertStrech();
  std::unique_ptr<QMenu> CreateDelayMenu();

  QToolButton* m_run_button{nullptr};
  QToolButton* m_pause_button{nullptr};
  QToolButton* m_step_button{nullptr};
  QToolButton* m_stop_button{nullptr};
  QToolButton* m_delay_button{nullptr};
  std::unique_ptr<QMenu> m_delay_menu;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_MONITORTREETOOLBAR_H
